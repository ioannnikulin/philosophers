/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eating.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 18:09:09 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/31 18:15:14 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

t_usec	prepare_to_eat(t_philo *p, int *errno)
{
	t_usec	before;

	if (*errno)
		return (p->props->tstart);
	assign(errno, 0, 0);
	tsull_set_release(&p->state, THINKS, TAKES, errno);
	if (*errno)
		return (p->props->tstart);
	before = mtime(&p->props->tstart, errno, p->props);
	if (*errno || smart_wait(p, before, errno) || *errno)
		return (before);
	if (p->i % 2 == 0)
	{
		if (take_fork(p, TOOK_L, 1, errno) || *errno)
			return (before);
	}
	if (take_fork(p, TOOK_R, 1, errno) || *errno)
		return (before);
	if (p->i % 2 == 1)
	{
		if (take_fork(p, TOOK_L, 1, errno) || *errno)
			return (before);
	}
	return (before);
}

int	eat(t_philo *p, int *errno)
{
	t_usec	started_eating;

	if (*errno)
		return (0);
	tsull_set_release(&p->state, TAKES, EATS, errno);
	if (*errno)
		return (assign(errno, 4, 0));
	started_eating = mtime(&p->props->tstart, errno, p->props);
	if (*errno || report(p, EATS, started_eating))
		return (assign(errno, 5, 0));
	tsusec_set(&p->last_meal, started_eating, errno);
	if (*errno)
		return (assign(errno, 7, 0));
	msleep(p->teat, errno, p->props);
	if (*errno)
		return (0);
	p->times_eaten ++;
	if (put_fork(p, TOOK_L, 1) || put_fork(p, TOOK_R, 1))
		return (assign(errno, 6, 0));
	return (0);
}
