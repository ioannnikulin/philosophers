/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   report.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 18:34:04 by inikulin          #+#    #+#             */
/*   Updated: 2024/05/26 20:06:29 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

int	report(t_philo *p, int action, int t)
{
	if (pthread_mutex_lock(p->props->print_poll))
		return (done(p->props, 0, "Failed to lock the print mutex", 1));
	if (t > MAX_MICROS || t < 0)
		return (done(p->props, 1, "Simulation time limit reached", 1));
	if (p->props->enough)
		return (done(p->props, 1, 0, 1));
	if (action == DIES)
	{
		printf("%i %i died\n", t, p->i);
		if (time(p->props.time_start) - t > DEAD_REPORT_MICROS)
			return (done(p->props, 1, "Reporting death too late", 1));
		return (done(p->props, 1, 0, 1));
	}
	else if (action == TAKES)
		printf("%i %i has taken a fork\n", t, p->i);
	else if (action == EATS)
	{
		printf("%i %i is eating\n", t, p->i);
		if (p->times_eaten + 1 == p->full_tgt &&
			++ p->props.full_philos == p->props.sz)
				return (done(p->props, 1, "Everybody has eaten enough", 1));
	}
	else if (action == sleeps)
		printf("%i %i is sleeping\n", t, p->i);
	else
		printf("%i %i is thinking\n", t, p->i);
	if (pthread_mutex_unlock(p->props->printpoll))
		return (done(props, 0, "Failed to unlock the print mutex", 1));
	return (0);
}
