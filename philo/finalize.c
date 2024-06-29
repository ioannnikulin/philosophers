/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:27:10 by inikulin          #+#    #+#             */
/*   Updated: 2024/06/29 18:05:43 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

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
		if (p->philos[i].i == -1)
			continue;
		ph = &p->philos[i];
		ret = ret | mutex(mode & DESTROY_M_LAST_MEAL, &ph->last_meal.m);
		ret = ret | mutex(mode & DESTROY_M_STATE, &ph->state.m);
		i ++;
	}
	if (mode & UNLOCK_PRINT)
		if (m_unlock(&p->print_poll))
			finalize(0, 0, TX_ERR_MUTEX_PRINT_UNLOCK, 0);
	ret = ret | mutex(mode & DESTROY_M_PRINT, &p->print_poll);
	if (ret)
		return (finalize(0, 0, TX_ERR_MUTEX_KILL, 1));
	return (finalize(0, 0, 0, 0));
}

int	finalize(t_props *p, int mode, char *msg, int ret)
{
	unsigned int	i;

	if (p)
	{
		if ((mode & REPORT_FATAL))
			tsint_set(&p->enough, ENOUGH, &p->errno);
		mutexes(p, mode);
		mfree(mode & FREE_PHILOS, (void*)&p->philos, sizeof(t_philo *) * p->sz, 0);
		mfree(mode & FREE_THREADS, (void*)&p->threads, sizeof(pthread_t *) * p->sz, 0);
		i = 0;
		while ((mode & DESTROY_M_FORKS) && p->forks && i < p->sz)
		{
			m_kill(&p->forks[i]);
			i ++;
		}
		mfree(mode & FREE_FORKS, (void*)&p->forks, sizeof(t_mutex *) * p->sz, 0);
	}
	if (msg)
		printf("%s\n", msg);
	if (mode & EXIT)
		exit(1);
	return (ret);
}
/* for perfect execution: message should be printed on print mutex */
