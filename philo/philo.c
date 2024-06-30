/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2024/06/30 15:32:46 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static int	frk(t_philo *p, pthread_mutex_t *f, int action)
{
	int	errno;

	errno = 0;
	if (action == 0)
	{
		if (m_lock(f))
			return (finalize(0, 0, msg(TX_ERR_FORK_TAKE, 0), 1));
		report(p, TAKES, mtime(&p->props->tstart, &errno, p->props));
		if (errno)
			return (1);
		return (0);
	}
	if (m_unlock(f))
		return (finalize(0, 0, msg(TX_ERR_FORK_PUT, 0), 1));
	return (0);
}

static t_usec	wait_n_eat(t_philo *p, int *errno)
{
	t_usec	b44k;
	t_usec	nwait;
	t_usec	started_eating;

	if (*errno || report(p, THINKS, mtime(&p->props->tstart, errno, p->props)) || *errno)
		return (assign(errno, 1, 0));
	b44k = mtime(&p->props->tstart, errno, p->props);
	if (*errno)
		return (0);
	msleep(p->wait, p->props);
	if (*errno || frk(p, p->l, 0) || frk(p, p->r, 0))
		return (assign(errno, 1, 0));
	tsint_set(&p->state, EATS, errno);
	if (*errno)
		return (0);
	started_eating = mtime(&p->props->tstart, errno, p->props);
	nwait = started_eating - b44k;
	if (*errno || report(p, EATS, started_eating))
		return (assign(errno, 1, 0));
	msleep(p->teat, p->props);
	p->times_eaten ++;
	tsusec_set(&p->last_meal, started_eating + p->teat, errno);
	if (*errno || frk(p, p->l, 1) || frk(p, p->r, 1))
		return (assign(errno, 1, 0));
	return (nwait);
}

static int	birth(t_philo **p, void *arg, int *errno)
{
	t_usec	first_meal;

	*p = (t_philo *)arg;
	assign(errno, 0, 0);
	if (*errno)
		return (1);
	first_meal = mtime(&(*p)->props->tstart, errno, (*p)->props);
	if (*errno)
		return (1);
	if (first_meal < DELAY)
		msleep(DELAY - first_meal, (*p)->props);
	first_meal = mtime(&(*p)->props->tstart, errno, (*p)->props);
	if (*errno)
		return (1);
	tsusec_set(&((*p)->last_meal), first_meal, errno);
	tsint_set(&((*p)->state), THINKS, errno);
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

static void *ret(int i, void *arg)
{
	printf("exiting %i\n", i);
	return (arg);
}

void	*philo(void *arg)
{
	t_philo	*p;
	t_usec	nwait;
	int		errno;
	int		state;

	if (birth(&p, arg, &errno))
		return (ret(p->i, arg));
	while (1)
	{
		nwait = wait_n_eat(p, &errno);
		if (errno)
			return (ret(p->i, arg));
		tsint_set(&p->state, SLEEPS, &errno);
		if (errno)
			return (0);
		if (report(p, SLEEPS, mtime(&p->props->tstart, &errno, p->props)) || errno)
			return (ret(p->i, arg));
		msleep(p->tsleep, p->props);
		restrat(p, nwait);
		state = tsint_get(&p->state, &errno);
		if (errno || state == ENOUGH || state == DIES)
			return (ret(p->i, arg));
	}
}
