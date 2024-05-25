/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 15:07:40 by inikulin          #+#    #+#             */
/*   Updated: 2024/05/25 19:39:26 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

int	main(int argc, char **argv)
{
	t_props	props;
	int		i;

	if (!init(&props, argc, argv))
		return (finalize(props, "Initialization error", 1));
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
