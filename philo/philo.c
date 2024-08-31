/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/31 18:25:40 by inikulin         ###   ########.fr       */
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
	tsusec_set(&((*p)->last_meal), first_meal, errno);
	if (*errno)
		return (4);
	tsull_set_release(&((*p)->state), NEWBORN, THINKS, errno);
	if (*errno)
		return (5);
	return (0);
}

static int	think(t_philo *p, int *errno)
{
	return (report(p, THINKS, mtime(&p->props->tstart, errno, p->props)));
}

static void	*philo_sleep(t_philo *p, int *errno)
{
	tsull_set_release(&p->state, EATS, SLEEPS, errno);
	if (*errno)
		return (philo_ret(p, 300 + *errno));
	if (report(p, SLEEPS, mtime(&p->props->tstart, errno, p->props)) || *errno)
		return (philo_ret(p, 10));
	msleep(p->tsleep, errno, p->props);
	if (*errno)
		return (philo_ret(p, 13));
	tsull_set_release(&p->state, SLEEPS, THINKS, errno);
	if (*errno)
		return (philo_ret(p, 11));
	return (0);
}

static void	*check_death(t_philo *p, int *errno)
{
	t_ull	state;

	state = tsull_get(&p->state, errno);
	if (*errno || (state & ANY_UNALIVE))
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
	int		errno;

	if (birth(&p, arg, &errno))
		return (philo_ret(p, 7));
	while (1)
	{
		if (errno || think(p, &errno) || errno)
			return (philo_ret(p, 8));
		prepare_to_eat(p, &errno);
		if (errno)
			return (philo_ret(p, errno));
		eat(p, &errno);
		if (errno || philo_sleep(p, &errno) || errno
			|| check_death(p, &errno) || errno)
			return (arg);
	}
}
