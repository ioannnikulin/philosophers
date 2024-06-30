/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   report.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 18:34:04 by inikulin          #+#    #+#             */
/*   Updated: 2024/06/30 15:12:07 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static int	act_die(t_philo *p, t_usec t)
{
	printf("%lli %lli %i died\n", mtime(&p->props->tstart, 0, p->props), t, p->i);
	if (mtime(&p->props->tstart, 0, p->props) - t > DEAD_REPORT_MICROS)
		return (finalize(p->props, REPORT_FATAL | UNLOCK_PRINT, msg(TX_LATE, t), 1));
	if (m_unlock(&p->props->print_poll))
		return (finalize(p->props, REPORT_FATAL, msg(TX_ERR_MUTEX_PRINT_UNLOCK, t), 1));
	return (finalize(p->props, REPORT_FATAL, msg(0, t), 0));
}

static int	act_eat(t_philo *p, t_usec t)
{
	printf("%lli %lli %i is eating\n", mtime(&p->props->tstart, 0, p->props), t, p->i);
	if (p->times_eaten + 1 == p->full_tgt)
	{
		p->props->full_philos ++;
		if (p->props->full_philos == p->props->sz)
			return (finalize(p->props, REPORT_FATAL | UNLOCK_PRINT, msg(TX_FULL, t), 1));
	}
	if (m_unlock(&p->props->print_poll))
		return (finalize(p->props, REPORT_FATAL, msg(TX_ERR_MUTEX_PRINT_UNLOCK, t), 1));
	return (0);
}

int	report(t_philo *p, int action, t_usec t)
{
	int	state;

	if (m_lock(&p->props->print_poll))
		return (finalize(p->props, REPORT_FATAL, msg(TX_ERR_MUTEX_PRINT_LOCK, t), 1));
	if (t > MAX_MICROS || t < 0)
		return (finalize(p->props, REPORT_FATAL | UNLOCK_PRINT, msg(TX_MAX_TIME, t), 1));
	if (tsint_get(&p->props->enough, &p->props->errno) == ENOUGH || p->props->errno)
		return (finalize(p->props, UNLOCK_PRINT, msg(0, 0), 1));
	state = tsint_get(&p->state, &p->props->errno);
	if (state == DIES || state == ENOUGH || p->props->errno)
		return (finalize(p->props, UNLOCK_PRINT, msg(0, 0), 1));
	if (action == DIES)
		return (act_die(p, t));
	else if (action == TAKES)
		printf("%lli %lli %i has taken a fork\n", mtime(&p->props->tstart, 0, p->props), t, p->i);
	else if (action == EATS)
		return (act_eat(p, t));
	else if (action == SLEEPS)
		printf("%lli %lli %i is sleeping\n", mtime(&p->props->tstart, 0, p->props), t, p->i);
	else if (action == THINKS)
		printf("%lli %lli %i is thinking\n", mtime(&p->props->tstart, 0, p->props), t, p->i);
	else
		printf("%lli %lli %i UNKNOWN ACTION %i\n", mtime(&p->props->tstart, 0, p->props), t, p->i, action);
	if (m_unlock(&p->props->print_poll))
		return (finalize(p->props, REPORT_FATAL, msg(TX_ERR_MUTEX_PRINT_UNLOCK, t), 1));
	return (0);
}
