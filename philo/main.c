/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 15:07:40 by inikulin          #+#    #+#             */
/*   Updated: 2024/05/26 20:34:35 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

int	usage(int ret)
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
has to eat to allow the simulation to end\n");
	return (ret);
}

int	main(int argc, char **argv)
{
	t_props	props;
	int		i;

	if (argc < 5 || argc > 6)
		return (usage(1));
	if (!init(&props, argc, argv))
		return (finalize(props, "Initialization error", 1));
	i = 1;
	props.time_start = time(0, &i);
	if (!i)
		return (finalize(props, "Timer error", 1));
	setup(props);
	i = -1;
	while (++ i < props.sz)
	{
		if (pthread_create(&props.threads[i], 0, &philo, &props.philos[i]))
			return (finalize(props, "Failed to start a thread", 1));
	}
	i = -1;
	while (++ i < props.sz)
	{
		if (pthread_join(&props.threads[i], 0))
			return (finalize(props, "Failed to join a thread", 1));
		if (props.philos[i].strat == DEAD)
			return (finalize(props, "Simulation over", 0));
	}
	return (finalize(props, "Simulation over", 0));
}
