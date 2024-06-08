/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   report.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 18:34:04 by inikulin          #+#    #+#             */
/*   Updated: 2024/06/08 20:42:59 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static int	act_die(t_philo *p, t_usec t)
{
	printf("%lli %i died\n", t, p->i);
	if (mtime(&p->props->tstart, 0) - t > DEAD_REPORT_MICROS)
		return (finalize(p->props, REPORT_FATAL | UNLOCK_PRINT, TX_LATE, 1));
	return (finalize(p->props, REPORT_FATAL | UNLOCK_PRINT, 0, 1));
}

int	report(t_philo *p, int action, t_usec t)
{
	if (m_lock(&p->props->print_poll))
		return (finalize(p->props, REPORT_FATAL, TX_ERR_MUTEX_PRINT_LOCK, 1));
	if (t > MAX_MICROS || t < 0)
		return (finalize(p->props, REPORT_FATAL | UNLOCK_PRINT, TX_MAX_TIME, 1));
	if (p->props->enough)
		return (finalize(0, 0, 0, 1));
	if (action == DIES)
		return (act_die(p, t));
	else if (action == TAKES)
		printf("%lli %i has taken a fork\n", t, p->i);
	else if (action == EATS)
	{
		printf("%lli %i is eating\n", t, p->i);
		if (p->times_eaten + 1 == p->full_tgt // should be not here, but in monitor
			&& ++ p->props->full_philos == p->props->sz)
			return (finalize(p->props, REPORT_FATAL | UNLOCK_PRINT, TX_FULL, 1));
	}
	else if (action == SLEEPS)
		printf("%lli %i is sleeping\n", t, p->i);
	else
		printf("%lli %i is thinking\n", t, p->i);
	if (m_unlock(&p->props->print_poll))
		return (finalize(p->props, REPORT_FATAL, TX_ERR_MUTEX_PRINT_UNLOCK, 1));
	return (0);
}
