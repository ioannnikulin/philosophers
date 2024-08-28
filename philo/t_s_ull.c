/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_s_ull.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 17:04:10 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/28 15:54:23 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

t_ull	tsull_get(t_s_ull *i, int *errno)
{
	t_ull	ret;

	assign(errno, 0, 0);
	if (m_lock(&i->m))
		return (finalize(0, 0, msg(i->e_lock, 0), assign(errno, 1, 0)));
	ret = i->v;
	return (ret);
}

int	tsull_release(t_s_ull *i, int *errno)
{
	if (errno && *errno)
		return (3);
	assign(errno, 0, 0);
	if (m_unlock(&i->m))
		return (finalize(0, 0, msg(i->e_unlock, 0), assign(errno, 1, 2)));
	return (0);
}

t_s_ull	*tsull_set_release(t_s_ull *i, t_ull oldval, t_ull newval, int *errno)
{
	if (*errno)
		return (0);
	assign(errno, 0, 0);
	tsull_set(i, oldval, newval, errno);
	if (*errno || tsull_release(i, errno) || *errno)
		return (0);
	return (i);
}

t_ull	tsull_get_release(t_s_ull *i, int *errno)
{
	t_ull	ret;

	if (*errno)
		return (0);
	ret = tsull_get(i, errno);
	if (*errno || tsull_release(i, errno) || *errno)
		return (0);
	return (ret);
}

t_s_ull	*tsull_set(t_s_ull *i, t_ull oldval, t_ull newval, int *errno)
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

t_s_ull	*tsull_add(t_s_ull *i, t_ull val, int *errno)
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

t_s_ull	*tsull_or_release(t_s_ull *i, t_ull val, int *errno)
{
	tsull_or(i, val, errno);
	if (*errno)
		return (i);
	tsull_release(i, errno);
	return (i);
}

t_s_ull	*tsull_nand_release(t_s_ull *i, t_ull val, int *errno)
{
	tsull_nand(i, val, errno);
	if (*errno)
		return (i);
	tsull_release(i, errno);
	return (i);
}

t_s_ull	*tsull_or(t_s_ull *i, t_ull val, int *errno)
{
	assign(errno, 0, 0);
	if (m_lock(&i->m))
	{
		finalize(0, 0, msg(i->e_lock, 0), assign(errno, 1, 0));
		return (i);
	}
	i->v |= val;
	return (i);
}

t_s_ull	*tsull_nand(t_s_ull *i, t_ull val, int *errno)
{
	assign(errno, 0, 0);
	if (m_lock(&i->m))
	{
		finalize(0, 0, msg(i->e_lock, 0), assign(errno, 1, 0));
		return (i);
	}
	t_ull old = i->v;
	old = old + 0;
	i->v &= ~val;
	if (i->v > 8192)
	{
		int abc = 0;
		abc ++;
	}
	return (i);
}