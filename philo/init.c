/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 18:34:04 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/26 12:57:09 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static int	prefill_philos(t_props *p)
{
	unsigned int	i;

	i = 0;
	while (i < p->sz)
	{
		p->philos[i].i = -1;
		p->philos[i].last_meal.v = -1;
		i ++;
	}
	return (0);
}

static int	parse_args(t_props *p, int argc, char **argv)
{
	int		errno;

	errno = 0;
	p->sz = atoui(argv[1], &errno);
	if (!p->sz || errno)
		return (usage(finalize(0, 0, msg(TX_ERR_NUM_FORMAT, 0, 0), 1)));
	p->philos = mcalloc(sizeof(t_philo) * p->sz);
	if (!p->philos)
		return (finalize(0, 0, msg(TX_ERR_MALLOC, 0, 0), 1));
	prefill_philos(p);
	p->philos[0].tdie = atoui(argv[2], &errno) * 1000;
	p->philos[0].teat = atoui(argv[3], &errno) * 1000;
	p->philos[0].tsleep = atoui(argv[4], &errno) * 1000;
	if (argc == 6)
		p->philos[0].full_tgt = atoui(argv[5], &errno);
	if (errno)
		return (usage(finalize(p, FREE_PHILOS,
					msg(TX_ERR_NUM_FORMAT, 0, 0), 1)));
	return (0);
}

static int	init_mutexes(t_philo *p)
{
	if (m_init(&p->state.m)
		|| m_init(&p->last_meal.m)
		|| m_init(&p->errno.m)
		|| m_init(&p->times_eaten.m)
	)
		return (1);
	p->last_meal.e_lock = TX_ERR_MUTEX_IND_LAST_MEAL_LOCK;
	p->last_meal.e_unlock = TX_ERR_MUTEX_IND_LAST_MEAL_UNLOCK;
	p->state.e_lock = TX_ERR_MUTEX_IND_STATE_LOCK;
	p->state.e_unlock = TX_ERR_MUTEX_IND_STATE_UNLOCK;
	p->errno.e_lock = TX_ERR_MUTEX_IND_ERRNO_LOCK;
	p->errno.e_unlock = TX_ERR_MUTEX_IND_ERRNO_UNLOCK;
	return (0);
}

static int	clone_philo(t_props *p, int f, int t)
{
	if (m_init(&(p->forks[t])) || init_mutexes(&p->philos[t]))
		return (finalize(p, STAGE_2, msg(TX_ERR_MUTEX_INIT, 0, 1), 1));
	p->philos[t].i = t;
	p->philos[t].l = &p->forks[t];
	p->philos[(p->sz + t - 1) % p->sz].r = &p->forks[t];
	p->philos[t].times_eaten.v = 0;
	p->philos[t].tdie = p->philos[f].tdie;
	p->philos[t].teat = p->philos[f].teat;
	p->philos[t].tsleep = p->philos[f].tsleep;
	p->philos[t].full_tgt = p->philos[f].full_tgt;
	p->philos[t].delta = 20;
	p->philos[t].props = p;
	p->philos[t].states.newborn = NEWBORN;
	p->philos[t].states.sleeps = SLEEPS;
	p->philos[t].states.eats = EATS;
	p->philos[t].states.thinks = THINKS;
	p->philos[t].states.takes = TAKES;
	p->philos[t].state.v = NEWBORN;
	p->philos[t].errno.v = 0;
	tsull_set_release(&p->philos[t].errno, 0, 0, 0);
	return (0);
}

int	init(t_props *p, int argc, char **argv)
{
	unsigned int	i;

	if (parse_args(p, argc, argv))
		return (1);
	p->threads = mcalloc(sizeof(pthread_t) * p->sz);
	p->forks = mcalloc(sizeof(t_mutex) * p->sz);
	if (!p->threads || !p->forks)
		return (finalize(p, STAGE_1, msg(TX_ERR_MALLOC, 0, 0), 1));
	if (m_init(&p->print_poll) || m_init(&p->errno.m))
		return (finalize(p, STAGE_1, msg(TX_ERR_MUTEX_INIT, 0, 0), 1));
	if (m_init(&p->mtime))
		return (finalize(p, STAGE_1 | DESTROY_M_TIME,
				msg(TX_ERR_MUTEX_INIT, 0, 1), 1));
	i = 0;
	p->enough.v = NEWBORN;
	while (i < p->sz)
		if (clone_philo(p, 0, i ++))
			return (1);
	return (0);
}
