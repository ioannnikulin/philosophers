/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_s_usec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 17:04:10 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/25 19:45:10 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

t_usec	tsusec_get(t_s_usec *i, t_s_ull *errno)
{
	t_usec	ret;

	if (tsull_get_release(errno, 0))
		return (0);
	if (m_lock(&i->m))
	{
		tsull_set_release(errno, 0, 1, 0);
		return (finalize(0, 0, msg(i->e_lock, 0, 1), 0));
	}
	ret = i->v;
	if (m_unlock(&i->m))
	{
		tsull_set_release(errno, 0, 2, 0);
		return (finalize(0, 0, msg(i->e_unlock, 0, 1), 0));
	}
	return (ret);
}

t_s_usec	*tsusec_set(t_s_usec *i, t_usec val, t_s_ull *errno)
{
	if (tsull_get_release(errno, 0))
		return (0);
	if (m_lock(&i->m))
	{
		tsull_set_release(errno, 0, 1, 0);
		finalize(0, 0, msg(i->e_lock, 0, 1), 0);
		return (i);
	}
	i->v = val;
	if (m_unlock(&i->m))
	{
		tsull_set_release(errno, 0, 2, 0);
		finalize(0, 0, msg(i->e_unlock, 0, 1), 0);
	}
	return (i);
}

t_s_usec	*tsusec_add(t_s_usec *i, t_usec val, t_s_ull *errno)
{
	if (tsull_get_release(errno, 0))
		return (0);
	if (m_lock(&i->m))
	{
		tsull_set_release(errno, 0, 1, 0);
		finalize(0, 0, msg(i->e_lock, 0, 1), 0);
		return (i);
	}
	i->v += val;
	if (m_unlock(&i->m))
	{
		tsull_set_release(errno, 0, 2, 0);
		finalize(0, 0, msg(i->e_unlock, 0, 1), 0);
	}
	return (i);
}
