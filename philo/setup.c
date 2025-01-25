/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:54:25 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/25 19:31:59 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

int	setup(t_props *p)
{
	unsigned int	i;
	time_t			t;
	int				e;

	t = p->philos[0].tdie - p->philos[0].tsleep - p->philos[0].teat;
	if (p->sz == 1)
	{
		p->philos[0].wait_before = t;
		p->philos[0].wait_period = 0;
		tsusec_set(&p->philos[0].last_meal, p->tstart, &p->errno);
		return (tsull_get_release(&p->errno, 0));
	}
	i = 0;
	while (i < p->sz)
	{
		p->philos[i].wait_before = (0.3 * (i % 2)) * t;
		p->philos[i].wait_period = t;
		tsusec_set(&p->philos[i ++].last_meal, p->tstart, &p->errno);
		e = tsull_get_release(&p->errno, 0);
		if (e)
			return (e);
	}
	if (p->sz % 2)
		p->philos[p->sz - 1].wait_before = 0.6 * t;
	return (0);
}
