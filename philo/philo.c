/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/24 18:11:41 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static int	birth(t_philo **p, void *arg, int *errno)
{
	t_usec	first_meal;

	*p = (t_philo *)arg;
	assign(errno, 0, 0);
	first_meal = mtime(&(*p)->props->tstart, errno, (*p)->props);
	if (*errno)
		return (1);
	if (first_meal < DELAY)
		msleep(DELAY - first_meal, (*p)->props);
	first_meal = mtime(&(*p)->props->tstart, errno, (*p)->props);
	if (*errno)
		return (1);
	tsusec_set(&((*p)->last_meal), first_meal, errno);
	if (*errno)
		return (1);
	tsint_set_release(&((*p)->state), NEWBORN, THINKS, errno);
	if (*errno)
		return (1);
	return (0);
}

static void	restrat(t_philo *p, t_usec nwait)
{
	(void)p;
	(void)nwait;
	/*if (nwait - p->wait > p->delta)
	{
		if (nwait > p->tdie * 0.8)
			p->wait = 0;
		else
			p->wait = 0;
	}*/
}

static void *ret(t_philo *p, int code)
{
	die_and_drop_forks(p, code != ENOUGH);
	if (m_lock(&p->props->print_poll))
	{
		finalize(p->props, REPORT_FATAL, msg(TX_ERR_MUTEX_PRINT_LOCK, 0), 1);
		return ((void*)p);
	}
	prints("\n", printlli(code, prints(" ", printlli(p->i, prints("exiting ", 0)))));
	if (m_unlock(&p->props->print_poll))
		finalize(p->props, REPORT_FATAL, msg(TX_ERR_MUTEX_PRINT_UNLOCK, 0), 1);
	return ((void*)p);
}

static int	think(t_philo *p, int *errno)
{
	return (report(p, THINKS, mtime(&p->props->tstart, errno, p->props)));
}

void	*philo(void *arg)
{
	t_philo	*p;
	t_usec	nwait;
	int		errno;
	int		state;

	if (birth(&p, arg, &errno))
		return (ret(p, 7));
	while (1)
	{
		if (errno || think(p, &errno) || errno)
			return (ret(p, 8));
		nwait = prepare_to_eat(p, &errno);
		if (errno)
			return (ret(p, errno));
		nwait = eat(p, &errno, nwait);
		if (errno)
			return (ret(p, errno));
		tsint_set_release(&p->state, EATS, SLEEPS, &errno);
		if (errno)
			return (ret(p, 9));
		if (report(p, SLEEPS, mtime(&p->props->tstart, &errno, p->props)) || errno)
			return (ret(p, 10));
		msleep(p->tsleep, p->props);
		tsint_set_release(&p->state, SLEEPS, THINKS, &errno);
		if (errno)
			return (ret(p, 11));
		restrat(p, nwait);
		state = tsint_get(&p->state, &errno);
		if (errno || state & ANY_UNALIVE)
		{
			tsint_release(&p->state, &errno);
			return (ret(p, 200 + state));
		}
		tsint_release(&p->state, &errno);
		if (errno)
			return (ret(p, 12));
	}
}
