/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   report.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 18:34:04 by inikulin          #+#    #+#             */
/*   Updated: 2024/06/02 17:35:20 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static int	act_die(t_philo *p, t_usec t)
{
	printf("%lli %i died\n", t, p->i);
	if (mtime(&p->props->tstart, 0) - t > DEAD_REPORT_MICROS)
		return (done(p->props, 1, "Reporting death too late", 1));
	return (done(p->props, 1, 0, 1));
}

int	report(t_philo *p, int action, t_usec t)
{
	if (pthread_mutex_lock(&p->props->print_poll))
		return (done(p->props, 0, "Failed to lock the print mutex", 1));
	if (t > MAX_MICROS || t < 0)
		return (done(p->props, 1, "Simulation time limit reached", 1));
	if (p->props->enough)
		return (done(p->props, 1, 0, 1));
	if (action == DIES)
		return (act_die(p, t));
	else if (action == TAKES)
		printf("%lli %i has taken a fork\n", t, p->i);
	else if (action == EATS)
	{
		printf("%lli %i is eating\n", t, p->i);
		if (p->times_eaten + 1 == p->full_tgt
			&& ++ p->props->full_philos == p->props->sz)
			return (done(p->props, 1, "Everybody has eaten enough", 1));
	}
	else if (action == SLEEPS)
		printf("%lli %i is sleeping\n", t, p->i);
	else
		printf("%lli %i is thinking\n", t, p->i);
	if (pthread_mutex_unlock(&p->props->print_poll))
		return (done(p->props, 0, "Failed to unlock the print mutex", 1));
	return (0);
}
