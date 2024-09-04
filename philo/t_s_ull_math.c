/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_s_ull_math.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 17:04:10 by inikulin          #+#    #+#             */
/*   Updated: 2024/09/04 16:14:28 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

t_s_ull	*tsull_add(t_s_ull *i, t_ull val, int *errno)
{
	assign(errno, 0, 0);
	if (m_lock(&i->m))
	{
		finalize(0, 0, msg(i->e_lock, 0, 1), assign(errno, 1, 0));
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
		finalize(0, 0, msg(i->e_lock, 0, 1), assign(errno, 1, 0));
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
		finalize(0, 0, msg(i->e_lock, 0, 1), assign(errno, 1, 0));
		return (i);
	}
	i->v &= ~val;
	return (i);
}
