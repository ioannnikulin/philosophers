/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2024/06/02 18:46:30 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static int	frk(t_philo *p, pthread_mutex_t *f, int action)
{
	int	ok;

	ok = 1;
	if (action == 0)
	{
		if (pthread_mutex_lock(f))
			return (done(p->props, 0, "Failed to take a fork", 1));
		report(p, TAKES, mtime(&p->props->tstart, &ok));
		return (0);
	}
	if (pthread_mutex_unlock(f))
		return (done(p->props, 0, "Failed to put a fork", 1));
	return (0);
}

static t_usec	wait_n_eat(t_philo *p, int *errno)
{
	t_usec	b44k;
	t_usec	nwait;

	if (*errno || report(p, THINKS, mtime(&p->props->tstart, errno)) || *errno)
		return (0);
	b44k = mtime(&p->props->tstart, errno);
	if (*errno)
		return (0);
	usleep(p->wait);
	if (*errno || frk(p, p->l, 0) || frk(p, p->r, 0))
		// if he dies waiting for a fork, we can be late to print this
		return (0);
	nwait = mtime(&p->props->tstart, errno) - b44k;
	if (*errno || report(p, EATS, nwait + b44k))
		return (0);
	usleep(p->teat);
	p->times_eaten ++;
	frk(p, p->l, 1);
	frk(p, p->r, 1);
	return (nwait);
}

void	*philo(void *arg)
{
	t_philo	*p;
	t_usec	nwait;

	p = (t_philo *)arg;
	while (1)
	{
		nwait = wait_n_eat(p, &p->props->errno);
		if (report(p, SLEEPS, mtime(&p->props->tstart, &p->props->errno))
			|| p->props->errno)
			return (0);
		usleep(p->tsleep);
		if (nwait - p->wait > p->delta)
		{
			if (nwait > p->tdie * 0.8)
				p->wait = 0;
			else
				p->wait = nwait;
		}
	}
}
