/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/24 14:37:35 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static int	end(t_props *p)
{
	unsigned int	i;

	i = 0;
	while (i < p->sz)
	{
		tsint_or(&p->philos[i].state, ENOUGH, &p->errno);
		i ++;
	}
	return (0);
}

static int	ret(t_props *p, int i, int if_ok)
{
	if (m_unlock(&p->philos[i].state.m))
		return (finalize(0, 0, msg(p->philos[i].state.e_unlock, 0), 4));
	return (if_ok);
}

static int	check(t_props *p)
{
	unsigned int	i;
	int				state;
	t_usec			last_meal;
	t_usec			hungry_for;

	i = 0;
	while (i < p->sz)
	{
		//report(&p->philos[i], BEFORE_INSPECTION, mtime(&p->tstart, &p->errno, p));
		if (m_lock(&p->philos[i].state.m))
			return (finalize(0, 0, msg(p->philos[i].state.e_lock, 0), 1));
		//report(&p->philos[i], INSIDE_INSPECTION, mtime(&p->tstart, &p->errno, p));
		state = p->philos[i].state.v;
		if (state & DIES)
			return (ret(p, i, DIES));
		if (state & NEWBORN)
		{
			i ++;
			continue;
		}
		last_meal = tsusec_get(&p->philos[i].last_meal, &p->errno);
		if (p->errno)
			return (ret(p, i, 2));
		hungry_for = mtime(&last_meal, &p->errno, p) - p->tstart;
		if (p->errno)
			return (ret(p, i, 3));
		if (hungry_for > p->philos[i].tdie)
		{
			report(&p->philos[i], DIES, mtime(&p->tstart, &p->errno, p));
			die_and_drop_forks(&p->philos[i]);
			return (DIES);
		}
		if (m_unlock(&p->philos[i].state.m))
			return (finalize(0, 0, msg(p->philos[i].state.e_unlock, 0), 4));
		i ++;
	}
	return (0);
} /* careful: DIES is 32*/

void	*moni(void *a)
{
	t_props	*p;
	t_usec	start;

	p = (t_props *)a;
	assign(&p->errno, 0, 0);
	start = mtime(&p->tstart, &p->errno, p);
	if (p->errno > 0)
		return (a);
	if (start < DELAY)
		msleep(DELAY - start, p);
	while (1)
	{
		if ((tsint_get(&p->enough, &p->errno) & ENOUGH) || p->errno > 0)
			break ;
		if (check(p))
			break ;
	}
	end(p);
	return (a);
}
