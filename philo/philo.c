/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/19 22:09:26 by inikulin         ###   ########.fr       */
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
	tsint_set(&((*p)->state), NEWBORN, THINKS, errno);
	if (*errno)
		return (1);
	return (0);
}

static void	restrat(t_philo *p, t_usec nwait)
{
	if (nwait - p->wait > p->delta)
	{
		if (nwait > p->tdie * 0.8)
			p->wait = 0;
		else
			p->wait = 0;
	}
}

static void *ret(int i, void *arg, t_props *p, int code)
{
	if (m_lock(&p->print_poll))
	{
		finalize(p, REPORT_FATAL, msg(TX_ERR_MUTEX_PRINT_LOCK, 0), 1);
		return (arg);
	}
	prints("\n", printlli(code, prints(" ", printlli(i, prints("exiting ", 0)))));
	if (m_unlock(&p->print_poll))
		finalize(p, REPORT_FATAL, msg(TX_ERR_MUTEX_PRINT_UNLOCK, 0), 1);
	return (arg);
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
		return (ret(p->i, arg, p->props, 0));
	while (1)
	{
		if (errno || think(p, &errno) || errno)
			return (ret(p->i, arg, p->props, 1));
		nwait = eat(p, &errno, prepare_to_eat(p, &errno));
		if (errno)
			return (ret(p->i, arg, p->props, 2));
		tsint_set(&p->state, EATS, SLEEPS, &errno);
		if (errno)
			return (ret(p->i, arg, p->props, 3));
		if (report(p, SLEEPS, mtime(&p->props->tstart, &errno, p->props)) || errno)
			return (ret(p->i, arg, p->props, 4));
		msleep(p->tsleep, p->props);
		restrat(p, nwait);
		state = tsint_get(&p->state, &errno);
		if (errno || state == ENOUGH || state == DIES)
			return (ret(p->i, arg, p->props, 10 + state));
	}
}
