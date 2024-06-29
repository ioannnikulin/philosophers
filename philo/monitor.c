/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2024/06/29 21:02:23 by inikulin         ###   ########.fr       */
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
		hungry_for = mmtime(&last_meal, &p->errno) - p->tstart;
		if (p->errno)
			return (2);
		if (hungry_for > p->philos[i].tdie)
		{
			printf("%lli %lli\n", last_meal, hungry_for);
			report(&p->philos[i], DIES, mtime(&p->tstart, &p->errno));
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

	p = (t_props *)a;
	while (1)
	{
		if (tsint_get(&p->enough, &p->errno) == ENOUGH || p->errno)
			break ;
		if (check(p))
			break ;
	}
	end(p);
	finalize(p, STAGE_2 | EXIT, msg(TX_OVER, 0), 0);
	return (a);
}
