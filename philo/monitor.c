/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2024/06/08 20:40:30 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static int	end(t_props *p, unsigned int i)
{
	if (m_lock(&p->philos[i].m_state))
		finalize(0, 0, TX_ERR_MUTEX_IND_STATE_LOCK, 0);
	p->philos[i].state = ENOUGH;
	if (m_unlock(&p->philos[i].m_state))
		finalize(0, 0, TX_ERR_MUTEX_IND_STATE_UNLOCK, 0);
	return (0);
}
static int	extract(t_s_int *var, int *errno)
{
	int	ret;

	if (m_lock(&m))
		finalize(0, 0, TX_ERR_MUTEX_IND_STATE_LOCK, 0);
	ret = *var;
	if (m_unlock(&p->philos[i].m_state))
		finalize(0, 0, TX_ERR_MUTEX_IND_STATE_UNLOCK, 0);
}
static int	check(t_props *p, unsigned int i)
{
}

static void *r(void *ret, int ignore)
{
	(void)ignore;
	return (ret);
}

void	*moni(void *a)
{
	t_props			*p;
	int				enough;
	unsigned int	i;

	p = (t_props *)a;
	while (1)
	{
		if (m_lock(&p->m_enough))
			return (r(a, finalize(0, REPORT_FATAL, TX_ERR_MUTEX_STATE_LOCK, 0)));
		enough = p->enough;
		if (m_unlock(&p->m_enough))
			return (r(a, finalize(0, REPORT_FATAL, TX_ERR_MUTEX_STATE_UNLOCK, 0)));
		i = 0;
		while (i < p->sz)
		{
			i += (enough && end(p, i)) & 0;
			i += (!enough && check(p, i)) & 0;
			i ++;
		}
		if (enough)
			break ;
	}
}
