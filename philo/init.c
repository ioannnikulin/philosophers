/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 18:34:04 by inikulin          #+#    #+#             */
/*   Updated: 2024/05/25 20:07:00 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

static int	usage(int ret)
{
	printf("Usage:./philo Q D E S F\n\
\t- Q (quantity)		- unsigned int, amount of philosophers (and forks)\n\
\t- D (dies)			- unsigned int, time in milliseconds since last meal \
(start of simulation considered a meal) a philosopher can live without food\n\
\t- E (eats)			- unsigned int, time in milliseconds it takes a \
philosopher to eat after acquiring a fork\n\
\t- S (sleeps)			- unsigned int, time in milliseconds a philosopher \
has to sleep passively right after finishing a meal\n\
\t- F (full; optional) 	- unsigned int, number of times every philosopher \
must eat before the simulation is ended\n");
	return (ret);
}

static unsigned int	atoui(const char *nptr, int *ok)
{
	unsigned long	res;
	size_t			cur;

	res = 0;
	cur = 0;
	while (nptr[cur] && (nptr[cur] > 47 && nptr[cur] < 58))
	{
		res = res * 10 + nptr[cur ++] - '0';
		if (res > UINT_MAX)
		{
			*ok = 0;
			return (0);
		}
	}
	*ok = *ok & 1;
	return (res);
}

static int	parse_args(t_props *p, int argc, char **argv)
{
	p->philos = 0;
	p->threads = 0;
	p->forks = 0;
	if (argc < 5 || argc > 6)
		return (usage(1));
	p = malloc(sizeof(t_props));
	if (!p)
		return (1);
	ok = 1;
	p->phil_props.sz = atoui(argv[1], &ok);
	p->phil_props.tdie = atoui(argv[2], &ok);
	p->phil_props.teat = atoui(argv[3], &ok);
	p->phil_props.tsleep = atoui(argv[4], &ok);
	p->phil_props.full_tgt = 0;
	p->phil_props.full_act = 0;
	if (argc == 6)
		p->phil_props.full_tgt = atoui(argv[5], &ok);
	if (!ok)
		return (usage(1));
	return (0);
}

int	init(t_props *p, int argc, char **argv)
{
	int	i;

	if (parse_args(p, argc, argv))
		return (1);
	p->philos = malloc(sizeof(t_philo) * p->sz);
	p->threads = malloc(sizeof(pthread_t) * p->sz);
	p->forks = malloc(sizeof(pthread_mutex_t) * p->sz);
	if (!p->philos || !p->threads || !p->forks)
		return (1);
	ok = -1;
	while (++ ok < p->sz)
	{
		if (pthread_mutex_init(&p->forks[ok], 0))
			return (1);
		p->philos[ok].i = ok;
		p->philos[ok].strat = STABLE;
		p->philos[ok].prevwait = 0;
		p->philos[ok].props = p;
	}
	if (pthread_mutex_init(&p->print.poll, 0))
		return (1);
	return (0);
}
