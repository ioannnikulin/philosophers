/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 18:34:04 by inikulin          #+#    #+#             */
/*   Updated: 2024/05/26 20:32:36 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static unsigned int	atoui(const char *nptr, int *ok)
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
			*ok = 0;
			return (0);
		}
		smth = 1;
	}
	if (smth == 0)
		*ok = 0;
	*ok = *ok & 1;
	return (res);
}

static int	parse_args(t_props *p, int argc, char **argv)
{
	int	ok;

	p->philos = 0;
	p->threads = 0;
	p->forks = 0;
	ok = 1;
	p->sz = atoui(argv[1], &ok);
	if (!p->sz)
		return (usage(1));
	p->philos = malloc(sizeof(t_philo) * p->sz);
	if (!p->philos)
		return (1);
	p->philos[0].tdie = atoui(argv[2], &ok);
	p->philos[0].teat = atoui(argv[3], &ok);
	p->philos[0].tsleep = atoui(argv[4], &ok);
	if (argc == 6)
		p->phil_props.full_tgt = atoui(argv[5], &ok);
	if (!ok)
	{
		free(p->philos);
		return (usage(1));
	}
	return (0);
}

static int	clone_philo(t_props *p, int f, int t)
{
	if (pthread_mutex_init(p->forks[t], 0))
		return (1);
	p->philos[t].i = i;
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
}

int	init(t_props *p, int argc, char **argv)
{
	int	i;

	if (parse_args(p, argc, argv))
		return (1);
	p->enough = 0;
	p->full_philos = 0;
	p->threads = malloc(sizeof(pthread_t) * p->sz);
	p->forks = malloc(sizeof(pthread_mutex_t) * p->sz);
	if (!p->threads || !p->forks)
	{
		free(p->threads);
		return (1);
	}
	i = -1;
	while (++ i < p->sz)
		clone_philo(p, 0, i);
	if (pthread_mutex_init(&p->print_poll, 0))
		return (1);
	return (0);
}
