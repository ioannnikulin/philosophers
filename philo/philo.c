/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/26 13:41:51 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static int	think(t_philo *p, t_s_ull *errno)
{
	return (report(p, THINKS, mtime(&p->props->tstart, errno, p->props)));
}

static void	*check_death(t_philo *p, t_s_ull *errno)
{
	t_ull	state;

	state = tsull_get(&p->state, errno);
	if (tsull_get_release(errno, 0) || (state & ANY_UNALIVE))
	{
		tsull_release(&p->state, errno);
		return (philo_ret(p, 200 + state));
	}
	tsull_release(&p->state, errno);
	return (0);
}

void	*body(t_philo *p, void *arg, int e)
{
	if (tsull_get_release(&p->errno, 0) || think(p, &p->errno)
		|| tsull_get_release(&p->errno, 0))
		return (philo_ret(p, 8));
	prepare_to_eat(p, &p->errno);
	e = tsull_get_release(&p->errno, 0);
	if (e)
		return (philo_ret(p, e));
	eat(p, &p->errno);
	if (tsull_get_release(&p->errno, 0))
		return (philo_ret(p, 10));
	arg = philo_sleep(p, &p->errno);
	if (arg)
		return (arg);
	arg = check_death(p, &p->errno);
	if (arg)
		return (arg);
	if (tsull_get_release(&p->errno, 0))
		return (philo_ret(p, 10));
	return (0);
}

void	*philo(void *arg)
{
	t_philo	*p;
	int		e;

	e = 0;
	if (birth(&p, arg))
		return (philo_ret(p, 7));
	while (1)
	{
		arg = body(p, arg, e);
		if (arg)
			return (arg);
	}
	return ((void *)p);
}
