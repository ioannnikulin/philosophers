/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 18:34:04 by inikulin          #+#    #+#             */
/*   Updated: 2024/06/02 18:48:35 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static unsigned int	atoui(const char *nptr, int *errno)
{
	unsigned long	res;
	size_t			cur;
	int				smth;

	res = 0;
	cur = 0;
	smth = 0;
	while (nptr[cur] && (nptr[cur] > 47 && nptr[cur] < 58))
	{
		res = res * 10 + nptr[cur ++] - '0';
		if (res > UINT_MAX)
		{
			*errno = 1;
			return (0);
		}
		smth = 1;
	}
	if (smth == 0)
	{
		*errno = 1;
		return (0);
	}
	return (res);
}

static int	parse_args(t_props *p, int argc, char **argv)
{
	p->philos = 0;
	p->threads = 0;
	p->forks = 0;
	p->sz = atoui(argv[1], &p->errno);
	if (!p->sz)
		return (usage(1));
	p->philos = malloc(sizeof(t_philo) * p->sz);
	if (!p->philos)
		return (1);
	p->philos[0].tdie = atoui(argv[2], &p->errno);
	p->philos[0].teat = atoui(argv[3], &p->errno);
	p->philos[0].tsleep = atoui(argv[4], &p->errno);
	if (argc == 6)
		p->philos[0].full_tgt = atoui(argv[5], &p->errno);
	if (p->errno)
	{
		free(p->philos);
		return (usage(1));
	}
	return (0);
}

static int	clone_philo(t_props *p, int f, int t)
{
	if (pthread_mutex_init(&(p->forks[t]), 0))
		return (1);
	p->philos[t].i = t;
	p->philos[t].l = &p->forks[t];
	p->philos[(p->sz + t - 1) % p->sz].r = &p->forks[t];
	p->philos[t].wait = 0;
	p->philos[t].props = p;
	p->philos[t].times_eaten = 0;
	p->philos[t].tdie = p->philos[f].tdie;
	p->philos[t].teat = p->philos[f].teat;
	p->philos[t].tsleep = p->philos[f].tsleep;
	p->philos[t].full_tgt = p->philos[f].full_tgt;
	p->philos[t].delta = 20;
	return (0);
}

int	init(t_props *p, int argc, char **argv)
{
	unsigned int	i;

	if (parse_args(p, argc, argv))
		return (1);
	p->enough = 0;
	p->full_philos = 0;
	p->errno = 0;
	p->threads = malloc(sizeof(pthread_t) * p->sz);
	p->forks = malloc(sizeof(pthread_mutex_t) * p->sz);
	if (!p->threads || !p->forks)
		return (1);
	i = 0;
	while (i < p->sz)
		if (clone_philo(p, 0, i ++))
			return (1);
	if (pthread_mutex_init(&p->print_poll, 0))
		return (1);
	return (0);
}
