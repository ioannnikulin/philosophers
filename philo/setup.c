/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:54:25 by inikulin          #+#    #+#             */
/*   Updated: 2024/06/29 16:43:45 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

int	setup(t_props *p)
{
	unsigned int	i;
	time_t			t;

	t = p->philos[0].tdie - p->philos[0].tsleep;
	if (p->sz == 1)
	{
		p->philos[0].wait = 0.8 * t;
		tsusec_set(&p->philos[0].last_meal, p->tstart, &p->errno);
		return (p->errno);
	}
	i = 0;
	while (i < p->sz)
	{
		p->philos[i].wait = (0.4 * (i % 2 + 1)) * t;
		tsusec_set(&p->philos[i ++].last_meal, p->tstart, &p->errno);
		if (p->errno)
			return (p->errno);
	}
	return (0);
}
