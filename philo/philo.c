/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/25 22:16:21 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static int	birth(t_philo **p, void *arg)
{
	t_usec	first_meal;

	*p = (t_philo *)arg;
	first_meal = mtime(&(*p)->props->tstart, &(*p)->errno, (*p)->props);
	if (tsull_get_release(&(*p)->errno, 0))
		return (1);
	tsusec_set(&((*p)->last_meal), first_meal, &(*p)->errno);
	if (tsull_get_release(&(*p)->errno, 0))
		return (4);
	tsull_set_release(&((*p)->state), &(*p)->states.newborn, THINKS,
		&(*p)->errno);
	if (tsull_get_release(&(*p)->errno, 0))
		return (5);
	return (0);
}

static int	think(t_philo *p, t_s_ull *errno)
{
	return (report(p, THINKS, mtime(&p->props->tstart, errno, p->props)));
}

static void	*philo_sleep(t_philo *p, t_s_ull *errno)
{
	int	e;

	tsull_set_release(&p->state, &p->states.eats, SLEEPS, errno);
	e = tsull_get_release(errno, 0);
	if (e)
		return (philo_ret(p, 300 + e));
	if (report(p, SLEEPS, mtime(&p->props->tstart, errno, p->props))
		|| tsull_get_release(errno, 0))
		return (philo_ret(p, 10));
	msleep(p->tsleep, errno, p->props);
	if (tsull_get_release(errno, 0))
		return (philo_ret(p, 13));
	tsull_set_release(&p->state, &p->states.sleeps, THINKS, errno);
	if (tsull_get_release(errno, 0))
		return (philo_ret(p, 11));
	return (0);
}

static void	*check_death(t_philo *p, t_s_ull *errno)
{
	t_ull	state;

	state = tsull_get(&p->state, errno);
	if (tsull_get_release(errno, 0) || (state & ANY_UNALIVE))
	{
		tsull_release(&p->state, errno);
		return (philo_ret(p, 200 + state));
	}
	tsull_release(&p->state, errno);
	return (0);
}

void	*philo(void *arg)
{
	t_philo	*p;
	int		e;

	if (birth(&p, arg))
		return (philo_ret(p, 7));
	while (1)
	{
		if (tsull_get_release(&p->errno, 0) || think(p, &p->errno) 
			|| tsull_get_release(&p->errno, 0))
			return (philo_ret(p, 8));
		prepare_to_eat(p, &p->errno);
		e = tsull_get_release(&p->errno, 0);
		if (e)
			return (philo_ret(p, e));
		eat(p, &p->errno);
		if (tsull_get_release(&p->errno, 0) || philo_sleep(p, &p->errno)
			|| tsull_get_release(&p->errno, 0) || check_death(p, &p->errno)
			|| tsull_get_release(&p->errno, 0))
			return (arg);
	}
}
