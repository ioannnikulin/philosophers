/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_s_ull.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 17:04:10 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/25 19:09:24 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

t_ull	tsull_get(t_s_ull *i, t_s_ull *errno)
{
	t_ull	ret;

	if (!i)
		return (0);
	if (errno && tsull_get_release(errno, 0))
		return (4);
	if (m_lock(&i->m))
	{
		tsull_set_release(errno, 0, 1, 0);
		return (finalize(0, 0, msg(i->e_lock, 0, 1), 0));
	}
	ret = i->v;
	return (ret);
}

int	tsull_release(t_s_ull *i, t_s_ull *errno)
{
	if (!i)
		return (0);
	if (errno && tsull_get_release(errno, 0))
		return (3);
	if (m_unlock(&i->m))
	{
		tsull_set_release(errno, 0, 1, 0);
		return (finalize(0, 0, msg(i->e_unlock, 0, 1), 2));
	}
	return (0);
}

t_s_ull	*tsull_set_release(t_s_ull *i, t_ull *oldval, t_ull newval,
	t_s_ull *errno)
{
	if (!i || tsull_get_release(errno, 0))
		return (0);
	tsull_set(i, oldval, newval, errno);
	if (tsull_get_release(errno, 0) || tsull_release(i, errno)
		|| tsull_get_release(errno, 0))
		return (0);
	return (i);
}

t_ull	tsull_get_release(t_s_ull *i, int *errno)
{
	t_ull	ret;

	if (!i)
		return (0);
	if (tsull_get_release(errno, 0))
		return (0);
	ret = tsull_get(i, errno);
	if (*errno || tsull_release(i, errno) || *errno)
		return (0);
	return (ret);
}

t_s_ull	*tsull_set(t_s_ull *i, t_ull *oldval, t_ull newval, int *errno)
{
	assign(errno, 0, 0);
	if (m_lock(&i->m))
	{
		finalize(0, 0, msg(i->e_lock, 0, 1), assign(errno, 1, 0));
		return (i);
	}
	if (oldval && !(*oldval & i->v))
	{
		assign(errno, 3, 0);
		return (i);
	}
	i->v = newval;
	return (i);
}
