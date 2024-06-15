/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2024/06/15 20:28:53 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static int	end(t_props *p)
{
	unsigned int	i;

	i = 0;
	while (i < p->sz)
	{
		tsint_set(&p->philos[i].state, ENOUGH, &p->errno);
		if (p->errno)
			return (1);
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
		last_meal = tsusec_get(&p->philos[i].last_meal, &p->errno);
		if (p->errno)
			return (2);
		hungry_for = mtime(&last_meal, &p->errno);
		if (p->errno)
			return (2);
		if (hungry_for > p->philos[i].tdie)
		{
			report(&p->philos[i], DIES, last_meal + p->philos[i].tdie);
			tsint_set(&p->philos[i].state, DIES, &p->errno);
			return (DIES);
		}
		i ++;
	}
	return (0);
}

void	*moni(void *a)
{
	t_props	*p;
	int		enough;

	p = (t_props *)a;
	while (1)
	{
		enough = tsint_get(&p->enough, &p->errno);
		if (p->errno)
		{
			tsint_set(&p->enough, ENOUGH, &p->errno);
			return (a);
		}
		if (enough)
		{
			end(p);
			return (a);
		}
		enough = check(p);
		if (enough)
			tsint_set(&p->enough, ENOUGH, &p->errno);
	}
}
