/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:54:25 by inikulin          #+#    #+#             */
/*   Updated: 2024/06/02 18:49:17 by inikulin         ###   ########.fr       */
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
		p->philos[0].last_meal = p->tstart;
		return (0);
	}
	i = 0;
	while (i < p->sz)
	{
		p->philos[i].wait = (0.4 * (i % 2 + 1)) * t;
		p->philos[i ++].last_meal = p->tstart;
	}
	return (0);
}
