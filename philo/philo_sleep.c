/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_sleep.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/26 13:52:27 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

void	*philo_sleep(t_philo *p, t_s_ull *errno)
{
	int	e;

	if (!tsull_set_release(&p->state, &p->states.eats, SLEEPS, errno))
		return (philo_ret(p, 15));
	e = tsull_get_release(errno, 0);
	if (e)
		return (philo_ret(p, 300 + e));
	if (report(p, SLEEPS, mtime(&p->props->tstart, errno, p->props))
		|| tsull_get_release(errno, 0))
		return (philo_ret(p, 10));
	msleep(p->tsleep, errno, p->props);
	if (tsull_get_release(errno, 0))
		return (philo_ret(p, 13));
	tsull_set_release(&p->state, &p->states.sleeps, THINKS, errno);
	if (tsull_get_release(errno, 0))
		return (philo_ret(p, 11));
	return (0);
}
