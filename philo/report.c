/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   report.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 18:34:04 by inikulin          #+#    #+#             */
/*   Updated: 2024/05/25 20:06:49 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

int	report(t_philo *p, int action)
{
	if (pthread_mutex_lock(p->props->printpoll))
		return (finalize(props, "Failed to lock the print mutex", 1));
	if (action == DIES)
	{
		printf("%i %i died\n", p->props->print.t, p->i);
		if (gettimeofday(p->props->print.t1, 0))
			return (finalize(props, "Failed to acquire time", 1));
		if (p->props->print.t1 - p->props->print.t > DEAD_REPORT_MICROS)
			return (finalize(p->props, "Reporting death too late", 1));
	}
	else if (action == TAKES)
		printf("%i %i has taken a fork\n", p->props->print.t, p->i);
	else if (action == EATS)
		printf("%i %i is eating\n", p->props->print.t, p->i);
	else if (action == sleeps)
		printf("%i %i is sleeping\n", p->props->print.t, p->i);
	else
		printf("%i %i is thinking\n", p->props->print.t, p->i);
	if (pthread_mutex_unlock(p->props->printpoll))
		return (finalize(props, "Failed to unlock the print mutex", 1));
	return (0);
}
