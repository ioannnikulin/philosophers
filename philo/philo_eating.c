/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eating.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/22 15:56:19 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static t_mutex	*mfork(t_philo *p, int which)
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

	this = mfork(p, which);
	other = mfork(p, TOOK_BOTH ^ which);
	if (!this)
		return (0);
	if (m_unlock(this))
	{
		put_fork(p, TOOK_BOTH ^ which, set_state);
		return (finalize(0, 0, msg(TX_ERR_FORK_PUT, 0), 1));
	}
	if (set_state)
	{
		tsint_nor(&p->state, which, &errno);
		if (errno)
		{
			put_fork(p, TOOK_BOTH ^ which, set_state);
			return (2);
		}
	}
	return (0);
}

static int	take_fork(t_philo *p, int which, int set_state)
{
	int		errno;
	t_mutex *this;
	t_mutex	*other;

	this = mfork(p, which);
	other = mfork(p, TOOK_BOTH ^ which);
	if (m_lock(this))
	{
		put_fork(p, TOOK_BOTH ^ which, set_state);
		return (finalize(0, 0, msg(TX_ERR_FORK_TAKE, 0), 1));
	}
	if ((tsint_get(&p->state, &errno) & ANY_UNALIVE) > 0 || errno)
	{
		put_fork(p, TOOK_BOTH ^ which, set_state);
		return (2);
	}
	if (set_state)
	{
		tsint_or(&p->state, which, &errno);
		if (errno)
		{
			put_fork(p, TOOK_BOTH ^ which, set_state);
			return (3);
		}
	}
	report(p, TAKES, mtime(&p->props->tstart, &errno, p->props));
	if (errno)
	{
		put_fork(p, TOOK_BOTH ^ which, set_state);
		return (4);
	}
	return (0);
}

t_usec	prepare_to_eat(t_philo *p, int *errno)
{
	t_usec	before;

	tsint_set(&p->state, THINKS, TAKES, errno);
	before = mtime(&p->props->tstart, errno, p->props);
	if (*errno)
		return (before);
	msleep(p->wait, p->props);
	if (take_fork(p, TOOK_L, 1))
	{
		assign(errno, 1, 0);
		return (before);
	}
	if (take_fork(p, TOOK_R, 1))
	{
		assign(errno, 2, 0);
		return (before);
	}
	return (before);
}

t_usec	eat(t_philo *p, int *errno, t_usec before)
{
	t_usec	nwait;
	t_usec	started_eating;

	if (*errno)
		return (0);
	tsint_set(&p->state, TAKES, EATS, errno);
	if (*errno) // errno enough
		return (0);
	started_eating = mtime(&p->props->tstart, errno, p->props);
	nwait = started_eating - before;
	if (*errno || report(p, EATS, started_eating))
		return (assign(errno, 1, 0));
	msleep(p->teat, p->props);
	p->times_eaten ++;
	tsusec_set(&p->last_meal, started_eating + p->teat, errno);
	if (*errno || put_fork(p, TOOK_L, 1) || put_fork(p, TOOK_R, 1))
		return (assign(errno, 1, 0));
	return (nwait);
}

int	die_and_drop_forks(t_philo *p)
{
	if (m_lock(&p->state.m))
		finalize(0, 0, msg(p->state.e_lock, 0), 1);
	if (p->state.v & TOOK_L)
		put_fork(p, TOOK_L, 0);
	if (p->state.v & TOOK_R)
		put_fork(p, TOOK_R, 0);
	p->state.v = DIES;
	if (m_unlock(&p->state.m))
		finalize(0, 0, msg(p->state.e_unlock, 0), 1);
	return (0);
}
