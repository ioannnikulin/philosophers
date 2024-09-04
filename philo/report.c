/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   report.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 18:34:04 by inikulin          #+#    #+#             */
/*   Updated: 2024/09/04 16:11:18 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static int	act_die(t_philo *p, t_usec t)
{
	print(mtime(&p->props->tstart, 0, p->props), t, p, DIES);
	if (mtime(&p->props->tstart, 0, p->props) - t > DEAD_REPORT_MICROS)
		return (finalize(p->props, REPORT_FATAL | UNLOCK_PRINT,
				msg(TX_LATE, t, 1), 1));
	if (m_unlock(&p->props->print_poll))
		return (finalize(p->props, REPORT_FATAL,
				msg(TX_ERR_MUTEX_PRINT_UNLOCK, t, 0), 1));
	return (finalize(p->props, REPORT_FATAL, msg(0, t, 0), 0));
}

int	report(t_philo *p, int state, t_usec t)
{
	if (m_lock(&p->props->print_poll))
		return (finalize(p->props, REPORT_FATAL,
				msg(TX_ERR_MUTEX_PRINT_LOCK, t, 0), 1));
	if (state == DIES)
		return (act_die(p, t));
	if (t > MAX_MICROS || t < 0)
		return (finalize(p->props, REPORT_FATAL | UNLOCK_PRINT,
				msg(TX_MAX_TIME, t, 1), 1));
	if ((tsull_get_release(&p->props->enough, &p->props->errno) & ENOUGH)
		|| p->props->errno)
		return (finalize(p->props, UNLOCK_PRINT, msg(0, 0, 0), 1));
	if ((state & ENOUGH) || p->props->errno)
		return (finalize(p->props, UNLOCK_PRINT, msg(0, 0, 0), 1));
	print(mtime(&p->props->tstart, 0, p->props), t, p, state);
	if ((state & EATS) && p->times_eaten + 1 == p->full_tgt)
	{
		p->props->full_philos ++;
		if (p->props->full_philos == p->props->sz)
			return (finalize(p->props, REPORT_FATAL | UNLOCK_PRINT,
					msg(ifc(PRINT_MODE == PRINT_FULL, TX_FULL, 0), t, 1), 1));
	}
	if (m_unlock(&p->props->print_poll))
		return (finalize(p->props, REPORT_FATAL,
				msg(TX_ERR_MUTEX_PRINT_UNLOCK, t, 0), 1));
	return (0);
}
