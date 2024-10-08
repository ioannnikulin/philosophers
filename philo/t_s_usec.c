/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_s_usec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 17:04:10 by inikulin          #+#    #+#             */
/*   Updated: 2024/09/04 16:14:49 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

t_usec	tsusec_get(t_s_usec *i, int *errno)
{
	t_usec	ret;

	assign(errno, 0, 0);
	if (m_lock(&i->m))
		return (finalize(0, 0, msg(i->e_lock, 0, 1), assign(errno, 1, 0)));
	ret = i->v;
	if (m_unlock(&i->m))
		return (finalize(0, 0, msg(i->e_unlock, 0, 1), assign(errno, 2, 0)));
	return (ret);
}

t_s_usec	*tsusec_set(t_s_usec *i, t_usec val, int *errno)
{
	assign(errno, 0, 0);
	if (m_lock(&i->m))
	{
		finalize(0, 0, msg(i->e_lock, 0, 1), assign(errno, 1, 0));
		return (i);
	}
	i->v = val;
	if (m_unlock(&i->m))
		finalize(0, 0, msg(i->e_unlock, 0, 1), assign(errno, 2, 0));
	return (i);
}

t_s_usec	*tsusec_add(t_s_usec *i, t_usec val, int *errno)
{
	assign(errno, 0, 0);
	if (m_lock(&i->m))
	{
		finalize(0, 0, msg(i->e_lock, 0, 1), assign(errno, 1, 0));
		return (i);
	}
	i->v += val;
	if (m_unlock(&i->m))
		finalize(0, 0, msg(i->e_unlock, 0, 1), assign(errno, 2, 0));
	return (i);
}
