/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:27:10 by inikulin          #+#    #+#             */
/*   Updated: 2024/06/08 20:05:56 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static int	m(int choice, t_mutex *m)
{
	return (choice && m && (m_kill(m) & mbzero(m, sizeof(t_mutex), 1)));
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
		if (!p->philos[i] || p->philos[i].i == -1)
			continue;
		ph = &p->philos[i];
		ret = ret | m(mode & DESTROY_M_TIMES_EATEN, &ph->m_times_eaten);
		ret = ret | m(mode & DESTROY_M_LAST_MEAL, &ph->m_last_meal);
		ret = ret | m(mode & DESTROY_M_DEAD, &ph->m_dead);
		i ++;
	}
	i = (mode & UNLOCK_PRINT) && m_unlock(&p->print_poll);
	ret = ret | m(mode & DESTROY_M_PRINT, &p->print_poll);
	return (finalize(0, 0, ret && TX_ERR_MUTEX_KILL, 1));
}

static int	fatal(t_props *p)
{
	if (m_lock(&p->state))
		return (finalize(0, 0, TX_ERR_MUTEX_STATE_LOCK, 1));
	p->state = ENOUGH;
	if (m_unlock(&p->state))
		return (finalize(0, 0, TX_ERR_MUTEX_STATE_UNLOCK, 1));
	return (0);
}

int	finalize(t_props *p, int mode, char *msg, int ret)
{
	unsigned int	i;
	int				ret;

	ret = 0;
	while (p)
	{
		if ((mode & REPORT_FATAL) && (fatal(p) >= 0))
			break ;
		mutexes(p, mode);
		ret = ret | mfree(mode & FREE_FILOS, &p->philos, sizeof(t_philo *), 0);
		ret = ret | mfree(mode & FREE_THREADS, &p->threads,
			sizeof(pthread_t *), 0);
		i = 0;
		while (p->forks && i < p->sz)
			ret = ret | (mode & DESTROY_M_FORKS) && m_kill(&p->forks[i]))
				| assign(i, i + 1, 0);
		ret = ret | mfree(mode & FREE_FORKS, &p->forks, sizeof(t_mutex *), 0);
		break ;
	}
	if (*msg)
		printf("%s\n", msg);
	return (ret);
}
