/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_s_int.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 17:04:10 by inikulin          #+#    #+#             */
/*   Updated: 2024/06/15 17:57:44 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

int	tsint_get(t_s_int *i, int *errno)
{
	int	ret;

	assign(errno, 0, 0);
	if (m_lock(i->m))
		return (finalize(0, 0, i->e_lock, assign(errno, 1, 0)));
	ret = i->v;
	if (m_unlock(i->m))
		return (finalize(0, 0, i->e_unlock, assign(errno, 2, 0)));
	return (ret);
}

t_s_int	*tsint_set(t_s_int *i, int val, int *errno)
{
	assign(errno, 0, 0);
	if (m_lock(i->m))
	{
		finalize(0, 0, i->e_lock, assign(errno, 1, 0));
		return (i);
	}
	i->v = val;
	if (m_unlock(i->m))
		finalize(0, 0, i->e_unlock, assign(errno, 2, 0));
	return (i);
}

t_s_int	*tsint_add(t_s_int *i, int val, int *errno)
{
	assign(errno, 0, 0);
	if (m_lock(i->m))
	{
		finalize(0, 0, i->e_lock, assign(errno, 1, 0));
		return (i);
	}
	i->v += val;
	if (m_unlock(i->m))
		finalize(0, 0, i->e_unlock, assign(errno, 2, 0));
	return (i);
}
