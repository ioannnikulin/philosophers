/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_smart_wait.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 18:09:09 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/31 18:27:25 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

#if PRINT_MODE == PRINT_FULL

static void	wait_time(t_philo *p, t_usec time)
{
	if (m_lock(&p->props->print_poll))
	{
		finalize(p->props, REPORT_FATAL, msg(TX_ERR_MUTEX_PRINT_LOCK, 0), 1);
		return ;
	}
	prints("\n", printull(time, prints(" for ", printlli(p->i,
					prints("waiting ", 0)))));
	if (m_unlock(&p->props->print_poll))
		finalize(p->props, REPORT_FATAL, msg(TX_ERR_MUTEX_PRINT_UNLOCK, 0), 1);
}
#endif
#if PRINT_MODE == PRINT_SUBMISSION

static void	wait_time(t_philo *p, t_usec time)
{
	(void)p;
	(void)time;
}
#endif

int	smart_wait(t_philo *p, t_usec before, int *errno)
{
	t_usec	towait;

	if (*errno)
		return (1);
	if (p->times_eaten == 0)
	{
		msleep(p->wait_before, errno, p->props);
		if (*errno)
			return (2);
		return (0);
	}
	towait = (p->wait_before + p->teat + p->tsleep + p->times_eaten
			* (p->wait_period + p->teat + p->tsleep) + p->wait_period);
	if (towait >= before)
	{
		towait -= before;
		wait_time(p, towait);
		if (towait > (p->tdie - p->tsleep - p->teat) * 0.7)
			towait = (p->tdie - p->tsleep - p->teat) * 0.7;
		msleep(towait, errno, p->props);
	}
	return (*errno);
}
