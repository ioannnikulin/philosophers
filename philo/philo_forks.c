/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_forks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 18:09:09 by inikulin          #+#    #+#             */
/*   Updated: 2024/09/04 16:16:44 by inikulin         ###   ########.fr       */
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
	int		errno;
	t_mutex	*this;
	t_mutex	*other;

	if (!(which & TOOK_BOTH))
		return (0);
	this = choose_fork(p, which);
	other = choose_fork(p, TOOK_BOTH ^ which);
	if (m_unlock(this))
	{
		finalize(0, 0, msg(TX_ERR_FORK_PUT, 0, 1), 1);
		put_fork(p, TOOK_BOTH ^ which, set_state);
		return (1);
	}
	if (set_state)
	{
		tsull_nand_release(&p->state, which, &errno);
		if (errno)
		{
			put_fork(p, TOOK_BOTH ^ which, set_state);
			return (2);
		}
	}
	return (0);
}

static int	set_state_proc(int set_state, t_philo *p, int which, int *errno)
{
	if (set_state)
	{
		tsull_or_release(&p->state, which, errno);
		if (*errno)
			return (assign(errno, 6, 6));
		return (0);
	}
	return (0);
}

int	take_fork(t_philo *p, int which, int set_state, int *errno)
{
	t_mutex	*this_m;
	t_mutex	*other_m;
	int		other;

	if (*errno)
		return (1);
	assign(errno, 0, 0);
	this_m = choose_fork(p, which);
	other = TOOK_BOTH ^ which;
	other_m = choose_fork(p, other);
	if (m_lock(this_m))
		return (assign(errno, 4, 4));
	if ((tsull_get(&p->state, errno) & ANY_UNALIVE) || *errno)
	{
		put_fork(p, which, 0);
		tsull_release(&p->state, errno);
		return (assign(errno, ENOUGH, ENOUGH));
	}
	tsull_release(&p->state, errno);
	if (set_state_proc(set_state, p, which, errno) || *errno)
		return (6);
	report(p, TAKES, mtime(&p->props->tstart, errno, p->props));
	if (*errno)
		return (assign(errno, 7, 7));
	return (0);
}
