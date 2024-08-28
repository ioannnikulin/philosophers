/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:27:10 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/28 20:46:26 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

t_usec	mtime(t_usec *t, int *errno, t_props *p)
{
	struct timeval	tv;
	t_usec			res;

	assign(errno, 0, 0);
	#ifdef TIMER_MUTEX
	if (m_lock(&p->mtime))
	{
		finalize(p, REPORT_FATAL, msg(TX_ERR_MUTEX_TIMER_LOCK, 0), 0);
		return (assign(errno, 1, 0));
	}
	#endif
	if (gettimeofday(&tv, 0))
	{
		finalize(p, REPORT_FATAL, msg(TX_ERR_TIMER, 0), 0);
		return (assign(errno, 1, 0));
	}
	res = tv.tv_sec * 1000000 + tv.tv_usec;
	if (t)
		res -= *t;
	#ifdef TIMER_MUTEX
	if (m_unlock(&p->mtime))
	{
		finalize(p, REPORT_FATAL, msg(TX_ERR_MUTEX_TIMER_UNLOCK, 0), 0);
		return (assign(errno, 1, 0));
	}
	#endif
	return (res);
}

void	msleep(t_usec t, int *errno, t_props *p)
{
	t_usec	start;
	t_usec	now;

	start = mtime(&p->tstart, errno, p);
	if (*errno)
	{
		finalize(p, REPORT_FATAL, msg(TX_ERR_SLEEP, 0), 0);
		return ;
	}
	while (!*errno)
	{
		now = mtime(&p->tstart, errno, p);
		if (now >= t + start) // TODO: introduce step as 10% of min time in input?
			break ;
		usleep(20);
	}
	if (*errno)
		finalize(p, REPORT_FATAL, msg(TX_ERR_SLEEP, 0), 0);
}
