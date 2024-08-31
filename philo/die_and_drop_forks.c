/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   die_and_drop_forks.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 16:12:06 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/31 16:12:14 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

int	die_and_drop_forks(t_philo *p, int block_first)
{
	if (block_first && m_lock(&p->state.m))
		return (1);
	if (p->state.v & (TOOK_L | EATS))
		put_fork(p, TOOK_L, 0);
	if (p->state.v & (TOOK_R | EATS))
		put_fork(p, TOOK_R, 0);
	p->state.v = DIES;
	tsull_release(&p->state, 0);
	return (0);
}
