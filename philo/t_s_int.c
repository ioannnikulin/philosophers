/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_s_int.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 17:04:10 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/22 20:58:49 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

int	tsint_get(t_s_int *i, int *errno)
{
	int	ret;

	assign(errno, 0, 0);
	if (m_lock(&i->m))
		return (finalize(0, 0, msg(i->e_lock, 0), assign(errno, 1, 0)));
	ret = i->v;
	if (m_unlock(&i->m))
		return (finalize(0, 0, msg(i->e_unlock, 0), assign(errno, 2, 0)));
	return (ret);
}

t_s_int	*tsint_set(t_s_int *i, int oldval, int newval, int *errno)
{
	assign(errno, 0, 0);
	if (m_lock(&i->m))
	{
		finalize(0, 0, msg(i->e_lock, 0), assign(errno, 1, 0));
		return (i);
	}
	if (!(oldval & i->v))
	{
		printlli(i->v, 0);
		prints("\n", 0);
		assign(errno, 3, 0);
		return (i);
	}
	i->v = newval;
	if (m_unlock(&i->m))
		finalize(0, 0, msg(i->e_unlock, 0), assign(errno, 2, 0));
	return (i);
}

t_s_int	*tsint_add(t_s_int *i, int val, int *errno)
{
	assign(errno, 0, 0);
	if (m_lock(&i->m))
	{
		finalize(0, 0, msg(i->e_lock, 0), assign(errno, 1, 0));
		return (i);
	}
	i->v += val;
	if (m_unlock(&i->m))
		finalize(0, 0, msg(i->e_unlock, 0), assign(errno, 2, 0));
	return (i);
}

t_s_int	*tsint_or(t_s_int *i, int val, int *errno)
{
	assign(errno, 0, 0);
	if (m_lock(&i->m))
	{
		finalize(0, 0, msg(i->e_lock, 0), assign(errno, 1, 0));
		return (i);
	}
	i->v |= val;
	if (m_unlock(&i->m))
		finalize(0, 0, msg(i->e_unlock, 0), assign(errno, 2, 0));
	return (i);
}

t_s_int	*tsint_nor(t_s_int *i, int val, int *errno)
{
	assign(errno, 0, 0);
	if (m_lock(&i->m))
	{
		finalize(0, 0, msg(i->e_lock, 0), assign(errno, 1, 0));
		return (i);
	}
	i->v |= ~val;
	if (m_unlock(&i->m))
		finalize(0, 0, msg(i->e_unlock, 0), assign(errno, 2, 0));
	return (i);
}
