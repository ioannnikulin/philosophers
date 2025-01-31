/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_smart_wait.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 18:09:09 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/29 17:17:28 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

#if PRINT_MODE == PRINT_FULL

static void	wait_time(t_philo *p, t_usec time)
{
	if (m_lock(&p->props->print_poll))
	{
		finalize(p->props, REPORT_FATAL, msg(TX_ERR_MUTEX_PRINT_LOCK, 0, 0), 1);
		return ;
	}
	prints("\n", printull(time, prints(" for ", printlli(p->i,
					prints("waiting ", 0)))));
	if (m_unlock(&p->props->print_poll))
		finalize(p->props, REPORT_FATAL, msg(TX_ERR_MUTEX_PRINT_UNLOCK, 0, 0),
			1);
}
#endif
#if PRINT_MODE == PRINT_SUBMISSION

static void	wait_time(t_philo *p, t_usec time)
{
	(void)p;
	(void)time;
}
#endif

int	smart_wait(t_philo *p, t_usec before, t_s_ull *errno)
{
	t_usec	towait;

	if (tsull_get_release(errno, 0))
		return (1);
	if (tsull_get_release(&p->times_eaten, errno) == 0)
	{
		msleep(p->wait_before, errno, p->props);
		if (tsull_get_release(errno, 0))
			return (2);
		return (0);
	}
	towait = (p->wait_before + tsull_get_release(&p->times_eaten, errno)
			* (p->wait_period + p->teat + p->tsleep)
			- p->wait_period * 0.2);
	wait_time(p, towait);
	wait_time(p, before);
	if (towait >= before)
	{
		towait -= before;
		if (towait > (p->tdie - p->tsleep - p->teat) * 0.7)
			towait = (p->tdie - p->tsleep - p->teat) * 0.7;
		wait_time(p, towait);
		msleep(towait, errno, p->props);
	}
	return (tsull_get_release(errno, 0));
}
