/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:27:10 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/29 17:09:46 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

t_usec	mtime(t_usec *t, t_s_ull *errno, t_props *p)
{
	struct timeval	tv;
	t_usec			res;

	if (gettimeofday(&tv, 0))
	{
		finalize(p, REPORT_FATAL, msg(TX_ERR_TIMER, 0, 1), 0);
		tsull_set_release(errno, 0, 1, 0);
		return (0);
	}
	res = tv.tv_sec * 1000000 + tv.tv_usec;
	if (t)
		res -= *t;
	return (res);
}

void	msleep(t_usec t, t_s_ull *errno, t_props *p)
{
	t_usec	start;
	t_usec	now;

	start = mtime(&p->tstart, errno, p);
	if (tsull_get_release(errno, 0))
	{
		finalize(p, REPORT_FATAL, msg(TX_ERR_SLEEP, 0, 1), 0);
		return ;
	}
	while (!tsull_get_release(errno, 0))
	{
		now = mtime(&p->tstart, errno, p);
		if (now >= t + start)
			break ;
		usleep(100);
	}
	if (tsull_get_release(errno, 0))
		finalize(p, REPORT_FATAL, msg(TX_ERR_SLEEP, 0, 1), 0);
}
