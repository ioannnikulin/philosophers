/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2024/05/26 20:30:49 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static int	fork(t_philo *p, pthread_mutex_t *f, int action)
{
	if (action == 0)
	{
		if (pthread_mutex_lock(f))
			return (done(p->props, 0, "Failed to take a fork", 1));
		report(p, TAKES);
		return (0);
	}
	if (pthread_mutex_unlock(f))
		return (done(p->props, 0, "Failed to put a fork", 1));
	return (0);
}

int	philo(void *arg)
{
	t_philo	*p;
	time_t	t;
	time_t	nwait;
	int		ok;

	p = (t_philo *)arg;
	ok = 1;
	while (1)
	{
		if (report(p, THINKS))
			return (1);
		t = time(&p->props.time_start, &ok);
		if (!ok)
			return (1);
		usleep(p->wait);
		if (fork(p, p->l, 0) || fork(p, p->r, 0))
			// if he dies waiting for a fork, we can be late to print this
			return (1);
		nwait = time(p->props.time_start) - t;
		if (nwait + p->tsleep > p->tdie)
			return (fork(p, p->l, 1) | fork(p, p->r, 1) | report(p, DIES));
		if (report(p, EATS))
			return (1);
		usleep(p->teat);
		p->times_eaten ++;
		if (fork(p, p->l, 1) || fork(p, p->r, 1) || report(p, SLEEPS))
			return (1);
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
