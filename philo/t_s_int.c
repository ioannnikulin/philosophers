/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_s_int.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 17:04:10 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/24 18:45:33 by inikulin         ###   ########.fr       */
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
	return (ret);
}

int	tsint_release(t_s_int *i, int *errno)
{
	if (errno && *errno)
		return (3);
	assign(errno, 0, 0);
	if (m_unlock(&i->m))
		return (finalize(0, 0, msg(i->e_unlock, 0), assign(errno, 1, 2)));
	return (0);
}

t_s_int	*tsint_set_release(t_s_int *i, int oldval, int newval, int *errno)
{
	if (*errno)
		return (0);
	assign(errno, 0, 0);
	tsint_set(i, oldval, newval, errno);
	if (*errno || tsint_release(i, errno) || *errno)
		return (0);
	return (i);
}

int	tsint_get_release(t_s_int *i, int *errno)
{
	int	ret;

	if (*errno)
		return (0);
	ret = tsint_get(i, errno);
	if (*errno || tsint_release(i, errno) || *errno)
		return (0);
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
		assign(errno, 3, 0);
		return (i);
	}
	i->v = newval;
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
	return (i);
}

t_s_int	*tsint_or_release(t_s_int *i, int val, int *errno)
{
	tsint_or(i, val, errno);
	if (*errno)
		return (i);
	tsint_release(i, errno);
	return (i);
}

t_s_int	*tsint_nor_release(t_s_int *i, int val, int *errno)
{
	tsint_nor(i, val, errno);
	if (*errno)
		return (i);
	tsint_release(i, errno);
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
	if (val == 64)
	{
		int abc = 1;
		abc ++;
	}
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
	return (i);
}
