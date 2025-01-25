/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 15:07:40 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/25 19:32:48 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

int	usage(int ret)
{
	return (prints("Usage:./philo Q D E S F\n\n\
\t- Q (quantity)		- unsigned int, amount of philosophers (and forks)\n\n\
\t- D (dies)		- unsigned int, time in milliseconds since start of last meal \
(start of simulation considered a meal) a philosopher can live without food\n\n\
\t- E (eats)		- unsigned int, time in milliseconds it takes a \
philosopher to eat after acquiring a fork\n\n\
\t- S (sleeps)		- unsigned int, time in milliseconds a philosopher \
has to sleep passively right after finishing a meal\n\n\
\t- F (full; optional) 	- unsigned int, number of times every philosopher \
has to eat to allow the simulation to end\n", ret));
}

int	start_philo(t_props *p, int i)
{
	return (pthread_create(&p->threads[i], 0, &philo, &p->philos[i]));
}

static int	start_all(t_props *props)
{
	unsigned int	i;

	i = 0;
	while (i < props->sz)
		if (start_philo(props, i ++))
			return (finalize(props, STAGE_2,
					msg(TX_ERR_THREAD_START, 0, 1), 1));
	if (pthread_create(&props->monitor, 0, moni, props))
		return (finalize(props, STAGE_2, msg(TX_ERR_THREAD_START, 0, 1), 1));
	return (0);
}

static int	join_all(t_props *props)
{
	unsigned int	i;

	i = 0;
	while (i < props->sz)
		if (pthread_join(props->threads[i ++], 0))
			return (finalize(props, STAGE_2, msg(TX_ERR_THREAD_JOIN, 0, 1), 1));
	if (pthread_join(props->monitor, 0))
		return (finalize(props, STAGE_2, msg(TX_ERR_THREAD_JOIN, 0, 1), 1));
	return (0);
}

int	main(int argc, char **argv)
{
	t_props			props;

	if (argc < 5 || argc > 6)
		return (usage(1));
	mbzero(&props, sizeof(t_props), 0);
	if (init(&props, argc, argv))
		return (1);
	props.tstart = mtime(0, &props.errno, &props);
	if (tsull_get_release(&props.errno, 0))
		return (finalize(&props, STAGE_2, msg(TX_ERR_TIMER, 0, 1), 1));
	setup(&props);
	if (start_all(&props))
		return (1);
	if (join_all(&props))
		return (1);
	return (finalize(&props, STAGE_2,
			msg(ifc(PRINT_MODE == PRINT_FULL, TX_OVER, 0), 0, 1), 0));
}
