/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   report.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 18:34:04 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/26 13:49:30 by inikulin         ###   ########.fr       */
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

static int	ass1(unsigned int *to, unsigned int val, int ret)
{
	if (to)
		*to = val;
	return (ret);
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
		|| tsull_get_release(&p->props->errno, 0))
		return (finalize(p->props, UNLOCK_PRINT, msg(0, 0, 0), 1));
	if ((state & ENOUGH) || tsull_get_release(&p->props->errno, 0))
		return (finalize(p->props, UNLOCK_PRINT, msg(0, 0, 0), 1));
	print(mtime(&p->props->tstart, 0, p->props), t, p, state);
	if ((state & EATS) && tsull_get_release(&p->times_eaten, &p->props->errno)
		+ 1 == p->full_tgt && ass1(&p->props->full_philos, p->props
			->full_philos + 1, 1) && p->props->full_philos >= p->props->sz)
		return (finalize(p->props, REPORT_FATAL | UNLOCK_PRINT,
				msg(ifc(PRINT_MODE == PRINT_FULL, TX_FULL, 0), t, 1), 1));
	if (m_unlock(&p->props->print_poll))
		return (finalize(p->props, REPORT_FATAL,
				msg(TX_ERR_MUTEX_PRINT_UNLOCK, t, 0), 1));
	return (0);
}
