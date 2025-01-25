/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/25 19:48:50 by inikulin         ###   ########.fr       */
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

static int	ret(t_philo *p, int if_ok)
{
	int	e;

	tsull_release(&p->state, &p->errno);
	e = tsull_get_release(&p->errno, 0);
	if (e)
		return (e);
	return (if_ok);
}

static int	died_hungry(t_philo *p, t_s_ull *errno)
{
	t_usec	last_meal;
	t_usec	hungry_for;

	last_meal = tsusec_get(&p->last_meal, errno);
	if (tsull_get_release(errno, 0))
		return (ret(p, 3));
	hungry_for = mtime(&last_meal, errno, p->props) - p->props->tstart;
	if (tsull_get_release(errno, 0))
		return (ret(p, 4));
	if (hungry_for > p->tdie)
	{
		report(p, DIES, mtime(&p->props->tstart, errno, p->props));
		die_and_drop_forks(p, 0);
		return (DIES);
	}
	return (0);
}

static int	check(t_props *p)
{
	unsigned int	i;
	int				state;

	i = 0;
	while (i < p->sz)
	{
		state = tsull_get(&p->philos[i].state, &p->errno);
		if (tsull_get_release(&p->errno, 0))
			return (1);
		if (state & (DIES | NEWBORN))
		{
			tsull_release(&p->philos[i ++].state, &p->errno);
			if (tsull_get_release(&p->errno, 0))
				return (2);
			if (state & DIES)
				return (DIES);
			continue ;
		}
		if (died_hungry(&p->philos[i], &p->errno))
			return (DIES);
		tsull_release(&p->philos[i ++].state, &p->errno);
		if (tsull_get_release(&p->errno, 0))
			return (5);
	}
	return (0);
}

void	*moni(void *a)
{
	t_props	*p;
	int		check_result;

	p = (t_props *)a;
	if (tsull_get_release(&p->errno, 0))
		return (a);
	while (1)
	{
		if ((tsull_get_release(&p->enough, &p->errno) & ENOUGH)
			|| tsull_get_release(&p->errno, 0))
			break ;
		check_result = check(p);
		if (check_result)
			break ;
	}
	end(p);
	return (a);
}
