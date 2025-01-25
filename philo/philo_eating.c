/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eating.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 18:09:09 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/25 21:17:00 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

t_usec	prepare_to_eat(t_philo *p, t_s_ull *errno)
{
	t_usec	before;

	if (tsull_get_release(errno, 0))
		return (p->props->tstart);
	tsull_set_release(&p->state, &p->states.thinks, TAKES, errno);
	if (tsull_get_release(errno, 0))
		return (p->props->tstart);
	before = mtime(&p->props->tstart, errno, p->props);
	if (tsull_get_release(errno, 0) || smart_wait(p, before, errno)
		|| tsull_get_release(errno, 0))
		return (before);
	if (p->i % 2 == 0)
	{
		if (take_fork(p, TOOK_L, 1, errno) || tsull_get_release(errno, 0))
			return (before);
	}
	if (take_fork(p, TOOK_R, 1, errno) || tsull_get_release(errno, 0))
		return (before);
	if (p->i % 2 == 1)
	{
		if (take_fork(p, TOOK_L, 1, errno) || tsull_get_release(errno, 0))
			return (before);
	}
	return (before);
}

int	eat(t_philo *p, t_s_ull *errno)
{
	t_usec	started_eating;

	if (tsull_get_release(errno, 0))
		return (0);
	tsull_set_release(&p->state, &p->states.takes, EATS, errno);
	if (tsull_get_release(errno, 0))
		return (ret_errno(errno, 4, 4));
	started_eating = mtime(&p->props->tstart, errno, p->props);
	if (tsull_get_release(errno, 0) || report(p, EATS, started_eating)
		|| tsull_get_release(errno, 0))
		return (ret_errno(errno, 5, 5));
	tsusec_set(&p->last_meal, started_eating, errno);
	if (tsull_get_release(errno, 0))
		return (ret_errno(errno, 6, 6));
	msleep(p->teat, errno, p->props);
	if (tsull_get_release(errno, 0))
		return (ret_errno(errno, 7, 7));
	p->times_eaten ++;
	if (put_fork(p, TOOK_L, 1) || put_fork(p, TOOK_R, 1))
		return (ret_errno(errno, 8, 8));
	return (0);
}
