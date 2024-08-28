/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:54:25 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/28 20:52:34 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

int	setup(t_props *p)
{
	unsigned int	i;
	time_t			t;

	t = p->philos[0].tdie - p->philos[0].tsleep - p->philos[0].teat;
	if (p->sz == 1)
	{
		p->philos[0].wait_before = t;
		p->philos[0].wait_period = 0;
		tsusec_set(&p->philos[0].last_meal, p->tstart, &p->errno);
		return (p->errno);
	}
	i = 0;
	while (i < p->sz)
	{
		p->philos[i].wait_before = (0.5 * (i % 2)) * t;
		p->philos[i].wait_period = t * 0.5;
		tsusec_set(&p->philos[i ++].last_meal, p->tstart, &p->errno);
		if (p->errno)
			return (p->errno);
	}
	return (0);
}
