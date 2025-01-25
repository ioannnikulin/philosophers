/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_forks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 18:09:09 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/25 21:32:34 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static t_mutex	*choose_fork(t_philo *p, int which)
{
	if (!(which & TOOK_BOTH))
		return (0);
	if (which & TOOK_L)
		return (p->l);
	return (p->r);
}

int	put_fork(t_philo *p, int which, int set_state)
{
	t_mutex	*this;

	if (!(which & TOOK_BOTH))
		return (0);
	this = choose_fork(p, which);
	if (m_unlock(this))
	{
		finalize(0, 0, msg(TX_ERR_FORK_PUT, 0, 1), 1);
		put_fork(p, TOOK_BOTH ^ which, set_state);
		return (1);
	}
	if (set_state)
	{
		tsull_nand_release(&p->state, which, &p->errno);
		if (tsull_get_release(&p->errno, 0))
		{
			put_fork(p, TOOK_BOTH ^ which, set_state);
			return (2);
		}
	}
	return (0);
}

static int	set_state_proc(int set_state, t_philo *p, int which, t_s_ull *errno)
{
	if (set_state)
	{
		tsull_or_release(&p->state, which, errno);
		if (tsull_get_release(errno, 0))
			return (ret_errno(errno, 6, 6));
		return (0);
	}
	return (0);
}

static int	ret(t_philo *p, int mode)
{
	put_fork(p, TOOK_L, 0);
	put_fork(p, TOOK_R, 0);
	if (mode == ENOUGH)
		tsull_release(&p->state, &p->errno);
	return (ret_errno(&p->errno, mode, mode));
}

int	take_fork(t_philo *p, int which, int set_state, t_s_ull *errno)
{
	t_mutex	*this_m;

	if (tsull_get_release(errno, 0))
		return (1);
	this_m = choose_fork(p, which);
	if (m_lock(this_m))
		return (ret_errno(errno, 4, 4));
	if ((tsull_get(&p->state, errno) & ANY_UNALIVE)
		|| tsull_get_release(errno, 0))
		return (ret(p, ENOUGH));
	tsull_release(&p->state, errno);
	if (set_state_proc(set_state, p, which, errno))
		return (ret(p, 6));
	report(p, TAKES, mtime(&p->props->tstart, errno, p->props));
	if (tsull_get_release(errno, 0))
		return (ret(p, 7));
	return (0);
}
