/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_birth.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/26 13:52:07 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

int	birth(t_philo **p, void *arg)
{
	t_usec	first_meal;

	*p = (t_philo *)arg;
	first_meal = mtime(&(*p)->props->tstart, &(*p)->errno, (*p)->props);
	if (tsull_get_release(&(*p)->errno, 0))
		return (1);
	tsusec_set(&((*p)->last_meal), first_meal, &(*p)->errno);
	if (tsull_get_release(&(*p)->errno, 0))
		return (4);
	tsull_set_release(&((*p)->state), &(*p)->states.newborn, THINKS,
		&(*p)->errno);
	if (tsull_get_release(&(*p)->errno, 0))
		return (5);
	return (0);
}
