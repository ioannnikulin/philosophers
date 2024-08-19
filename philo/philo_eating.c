/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eating.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/19 22:12:13 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static int	put_fork(t_philo *p, t_mutex *f, t_mutex *otherf)
{
	if (!f)
		return (0);
	if (m_unlock(f))
	{
		put_fork(p, otherf, 0);
		return (finalize(0, 0, msg(TX_ERR_FORK_PUT, 0), 1));
	}
	return (0);
}

static int	take_fork(t_philo *p, t_mutex *f, t_mutex *otherf)
{
	int	errno;

	errno = 0;
	if (m_lock(f))
	{
		put_fork(p, otherf, 0);
		return (finalize(0, 0, msg(TX_ERR_FORK_TAKE, 0), 1));
	}
	if ((tsint_get(&p->state, &errno) & ANY_UNALIVE) > 0 || errno)
	{
		put_fork(p, otherf, 0);
		return (2);
	}
	report(p, TAKES, mtime(&p->props->tstart, &errno, p->props));
	if (errno)
	{
		put_fork(p, otherf, 0);
		return (3);
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
	if (take_fork(p, p->l, 0))
	{
		assign(errno, 1, 0);
		return (before);
	}
	tsint_set(&p->state, TAKES, TOOK_L, errno);
	if (errno)
		return (before);
	if (take_fork(p, p->r, p->l))
	{
		assign(errno, 2, 0);
		return (before);
	}
	tsint_set(&p->state, TAKES, TOOK_BOTH, errno);
	return (before);
}

t_usec	eat(t_philo *p, int *errno, t_usec before)
{
	t_usec	nwait;
	t_usec	started_eating;

	if (*errno)
		return (0);
	tsint_set(&p->state, THINKS, EATS, errno);
	if (*errno) // errno enough
		return (0);
	started_eating = mtime(&p->props->tstart, errno, p->props);
	nwait = started_eating - before;
	if (*errno || report(p, EATS, started_eating))
		return (assign(errno, 1, 0));
	msleep(p->teat, p->props);
	p->times_eaten ++;
	tsusec_set(&p->last_meal, started_eating + p->teat, errno);
	if (*errno || put_fork(p, p->l, p->r) || put_fork(p, p->r, 0))
		return (assign(errno, 1, 0));
	return (nwait);
}

int	dead_dont_need_forks(t_philo *p)
{
// tsint_bitwise or, and add DEAD to what it holds
}
