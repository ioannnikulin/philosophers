/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/22 15:31:07 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static int	end(t_props *p)
{
	unsigned int	i;

	i = 0;
	while (i < p->sz)
	{
		tsint_set(&p->philos[i].state, ANY, ENOUGH, &p->errno);
		i ++;
	}
	return (0);
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
		state = tsint_get(&p->philos[i].state, &p->errno);
		if (p->errno)
			return (2);
		if (state == DIES)
			return (DIES);
		if (state == EATS || state == NEWBORN)
			return (0);
		last_meal = tsusec_get(&p->philos[i].last_meal, &p->errno);
		if (p->errno)
			return (2);
		hungry_for = mtime(&last_meal, &p->errno, p) - p->tstart;
		if (p->errno)
			return (2);
		if (hungry_for > p->philos[i].tdie)
		{
			report(&p->philos[i], DIES, mtime(&p->tstart, &p->errno, p));
			die_and_drop_forks(&p->philos[i]);
			return (DIES);
		}
		i ++;
	}
	return (0);
}

void	*moni(void *a)
{
	t_props	*p;

	p = (t_props *)a;
	while (1)
	{
		if (tsint_get(&p->enough, &p->errno) == ENOUGH || p->errno)
			break ;
		if (check(p))
			break ;
	}
	end(p);
	return (a);
}
