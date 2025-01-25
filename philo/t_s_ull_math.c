/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_s_ull_math.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 17:04:10 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/25 19:41:36 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

t_s_ull	*tsull_add(t_s_ull *i, t_ull val, t_s_ull *errno)
{
	if (!i || tsull_get_release(errno, 0))
		return (0);
	if (m_lock(&i->m))
	{
		tsull_set_release(errno, 0, 1, 0);
		finalize(0, 0, msg(i->e_lock, 0, 1), 0);
		return (0);
	}
	i->v += val;
	return (i);
}

t_s_ull	*tsull_or_release(t_s_ull *i, t_ull val, t_s_ull *errno)
{
	if (!i || tsull_get_release(errno, 0))
		return (0);
	tsull_or(i, val, errno);
	if (tsull_get_release(errno, 0) || tsull_release(i, errno)
		|| tsull_get_release(errno, 0))
		return (0);
	return (i);
}

t_s_ull	*tsull_nand_release(t_s_ull *i, t_ull val, t_s_ull *errno)
{
	if (!i || tsull_get_release(errno, 0))
		return (0);
	tsull_nand(i, val, errno);
	if (tsull_get_release(errno, 0) || tsull_release(i, errno)
		|| tsull_get_release(errno, 0))
		return (0);
	return (i);
}

t_s_ull	*tsull_or(t_s_ull *i, t_ull val, t_s_ull *errno)
{
	if (!i || tsull_get_release(errno, 0))
		return (0);
	if (m_lock(&i->m))
	{
		tsull_set_release(errno, 0, 1, 0);
		finalize(0, 0, msg(i->e_lock, 0, 1), 0);
		return (i);
	}
	i->v |= val;
	return (i);
}

t_s_ull	*tsull_nand(t_s_ull *i, t_ull val, t_s_ull *errno)
{
	if (!i || tsull_get_release(errno, 0))
		return (0);
	if (m_lock(&i->m))
	{
		tsull_set_release(errno, 0, 1, 0);
		finalize(0, 0, msg(i->e_lock, 0, 1), 0);
		return (i);
	}
	i->v &= ~val;
	return (i);
}
