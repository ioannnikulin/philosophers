/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:27:10 by inikulin          #+#    #+#             */
/*   Updated: 2024/09/04 16:57:14 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

t_fin_param	msg(char *msg, t_usec time, int lock_print)
{
	t_fin_param	res;

	res.msg = msg;
	res.time = time;
	res.lock_print = lock_print;
	return (res);
}

static int	mutex(int choice, t_mutex *m)
{
	if (choice && m)
	{
		if (m_kill(m))
			return (1);
		mbzero(m, sizeof(t_mutex), 0);
	}
	return (0);
}

static int	mutexes(t_props *p, int mode)
{
	unsigned int	i;
	int				ret;
	t_philo			*ph;

	i = 0;
	ret = 0;
	while (i < p->sz)
	{
		if (p->philos[i ++].i == -1)
		{
			continue ;
		}
		ph = &p->philos[i - 1];
		ret = ret | mutex(mode & DESTROY_M_LAST_MEAL, &ph->last_meal.m);
		ret = ret | mutex(mode & DESTROY_M_STATE, &ph->state.m);
	}
	if (mode & UNLOCK_PRINT)
		if (m_unlock(&p->print_poll))
			finalize(0, 0, msg(TX_ERR_MUTEX_PRINT_UNLOCK, 0, 0), 0);
	ret = ret | mutex(mode & DESTROY_M_PRINT, &p->print_poll);
	ret = ret | mutex(mode & DESTROY_M_TIME, &p->mtime);
	return (0);
}

static int	msg_processing(t_props *p, t_fin_param msgp)
{
	if (!msgp.time && p)
		msgp.time = mtime(&p->tstart, &p->errno, p);
	if (msgp.lock_print && m_lock(&p->print_poll))
		return (finalize(p, REPORT_FATAL, msg(TX_ERR_MUTEX_PRINT_LOCK, 0, 0), 1));
	prints("\n", prints(msgp.msg, prints(": ", printlli(msgp.time, 0))));
	if (msgp.lock_print && m_unlock(&p->print_poll))
		return (finalize(p, REPORT_FATAL, 
				msg(TX_ERR_MUTEX_PRINT_UNLOCK, 0, 0), 1));
	return (0);
}

int	finalize(t_props *p, int mode, t_fin_param msgp, int ret)
{
	unsigned int	i;

	if (p)
	{
		if ((mode & REPORT_FATAL))
			tsull_or_release(&p->enough, ENOUGH, &p->errno);
		mutexes(p, mode);
		mfree(mode & FREE_PHILOS, (void *)&p->philos,
			sizeof(t_philo) * p->sz, 0);
		mfree(mode & FREE_THREADS, (void *)&p->threads,
			sizeof(pthread_t *) * p->sz, 0);
		i = 0;
		while ((mode & DESTROY_M_FORKS) && p->forks && i < p->sz)
		{
			m_kill(&p->forks[i]);
			i ++;
		}
		mfree(mode & FREE_FORKS, (void *)&p->forks,
			sizeof(t_mutex *) * p->sz, 0);
	}
	if (msgp.msg)
		msg_processing(p, msgp);
	return (ret);
}
