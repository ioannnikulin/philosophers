/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/31 14:05:04 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static int	end(t_props *p)
{
	unsigned int	i;

	i = 0;
	while (i < p->sz)
	{
		tsull_or_release(&p->philos[i].state, ENOUGH, &p->errno);
		i ++;
	}
	return (0);
}

static int	ret(t_props *p, int i, int if_ok)
{
	int	errno;

	errno = 0;
	tsull_release(&p->philos[i].state, &errno);
	if (errno)
		return (errno);
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
		//report(&p->philos[i], INSIDE_INSPECTION, mtime(&p->tstart, &p->errno, p));
		state = tsull_get(&p->philos[i].state, &p->errno);
		if (p->errno)
			return (1);
		if (state & (DIES | NEWBORN))
		{
			tsull_release(&p->philos[i].state, &p->errno);
			if (p->errno)
				return (2);
			if (state & DIES)
				return (DIES);
			i ++;
			continue;
		}
		last_meal = tsusec_get(&p->philos[i].last_meal, &p->errno); // no need to block last meal anymore, its blocked by state anyway
		if (p->errno)
			return (ret(p, i, 3));
		hungry_for = mtime(&last_meal, &p->errno, p) - p->tstart;
		if (p->errno)
			return (ret(p, i, 4));
		if (hungry_for > p->philos[i].tdie)
		{
			report(&p->philos[i], DIES, mtime(&p->tstart, &p->errno, p));
			die_and_drop_forks(&p->philos[i], 0);
			return (DIES);
		}
		tsull_release(&p->philos[i].state, &p->errno);
		if (p->errno)
			return (5);
		i ++;
	}
	return (0);
} /* careful: DIES is 32*/

void	*moni(void *a)
{
	t_props	*p;
	t_usec	start;
	int		check_result;

	p = (t_props *)a;
	assign(&p->errno, 0, 0);
	start = mtime(&p->tstart, &p->errno, p);
	if (p->errno > 0)
		return (a);
	/* if (start < DELAY) */
		/* msleep(DELAY - start, &p->errno, p); */
	/* if (p->errno) */
		/* return (a); */
	while (1)
	{
		if ((tsull_get_release(&p->enough, &p->errno) & ENOUGH) || p->errno > 0)
			break ;
		check_result = check(p);
		if (check_result)
			break ;
	}
	end(p);
	return (a);
}
