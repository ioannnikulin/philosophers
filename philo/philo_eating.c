/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eating.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/24 19:00:33 by inikulin         ###   ########.fr       */
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

static int	put_fork(t_philo *p, int which, int set_state)
{
	int		errno;
	t_mutex *this;
	t_mutex	*other;

	if (which == 0)
		return (0);
	this = choose_fork(p, which);
	other = choose_fork(p, TOOK_BOTH ^ which);
	if (!this)
		return (0);
	if (m_unlock(this))
	{
		finalize(0, 0, msg(TX_ERR_FORK_PUT, 0), 1);
		put_fork(p, TOOK_BOTH ^ which, set_state);
		return (1);
	}
	if (set_state)
	{
		tsint_nor_release(&p->state, which, &errno);
		if (errno)
		{
			put_fork(p, TOOK_BOTH ^ which, set_state);
			return (2);
		}
	}
	return (0);
}

static int	take_fork(t_philo *p, int which, int set_state, int *errno)
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
	report(p, LOOKS, mtime(&p->props->tstart, errno, p->props)); // debug
	if (*errno)
		return (assign(errno, 2, 2));
	if (which == TOOK_R) // debug
		report(p, LOOKS, mtime(&p->props->tstart, errno, p->props));
	if (*errno)
		return (assign(errno, 3, 3));
	if (m_lock(this_m))
	{
		msg(TX_ERR_FORK_TAKE, 0);
		return (assign(errno, 4, 4));
	}
	if ((tsint_get(&p->state, errno) & ANY_UNALIVE) || *errno)
	{
		put_fork(p, which, 0);
		return (assign(errno, ENOUGH, ENOUGH));
	}
	tsint_release(&p->state, errno);
	if (set_state)
	{
		tsint_or_release(&p->state, which, errno);
		if (*errno)
			return (assign(errno, 6, 6));
	}
	report(p, TAKES, mtime(&p->props->tstart, errno, p->props));
	if (*errno)
		return (assign(errno, 7, 7));
	return (0);
}

/* debugging point */
static t_usec	ret(t_usec val, int *errno)
{
	if (*errno == 64)
	{
		int abc = 0;
		abc ++;
	}
	return (val);
}

t_usec	prepare_to_eat(t_philo *p, int *errno)
{
	t_usec	before;

	if (*errno)
		return (ret(p->props->tstart, errno));
	assign(errno, 0, 0);
	tsint_set_release(&p->state, THINKS, TAKES, errno);
	if (*errno)
		return (ret(p->props->tstart, errno));
	before = mtime(&p->props->tstart, errno, p->props);
	if (*errno)
		return (ret(before, errno));
	if (p->times_eaten == 0)
		msleep(p->wait_before, p->props);
	else
		msleep(p->wait_period, p->props);
	if (take_fork(p, TOOK_L, 1, errno) || *errno)
		return (ret(before, errno));
	if (take_fork(p, TOOK_R, 1, errno) || *errno)
		return (ret(before, errno));
	return (ret(before, errno));
}

t_usec	eat(t_philo *p, int *errno, t_usec before)
{
	t_usec	nwait;
	t_usec	started_eating;

	if (*errno)
		return (0);
	tsint_set_release(&p->state, TAKES, EATS, errno);
	if (*errno) // errno enough
		return (assign(errno, 4, 0));
	started_eating = mtime(&p->props->tstart, errno, p->props);
	nwait = started_eating - before;
	if (*errno || report(p, EATS, started_eating))
		return (assign(errno, 5, 0));
	msleep(p->teat, p->props);
	p->times_eaten ++;
	tsusec_set(&p->last_meal, started_eating + p->teat, errno);
	if (*errno || put_fork(p, TOOK_L, 1) || put_fork(p, TOOK_R, 1))
		return (assign(errno, 6, 0));
	return (nwait);
}

/* no state changes needed with this */
int	die_and_drop_forks(t_philo *p, int block_first)
{
	if (block_first && m_lock(&p->state.m))
		return (1);
	if (p->state.v & TOOK_L)
		put_fork(p, TOOK_L, 0);
	if (p->state.v & TOOK_R)
		put_fork(p, TOOK_R, 0);
	p->state.v = DIES;
	tsint_release(&p->state, 0);
	return (0);
}
