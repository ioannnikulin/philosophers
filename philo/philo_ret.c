/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_ret.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:53:57 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/31 18:39:27 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

#if PRINT_MODE == PRINT_FULL

void	*philo_ret(t_philo *p, int code)
{
	die_and_drop_forks(p, 0);
	if (m_lock(&p->props->print_poll))
	{
		finalize(p->props, REPORT_FATAL, msg(TX_ERR_MUTEX_PRINT_LOCK, 0), 1);
		return ((void *)p);
	}
	prints("\n", printlli(code, prints(" ", printlli(p->i,
					prints("exiting ", 0)))));
	if (m_unlock(&p->props->print_poll))
		finalize(p->props, REPORT_FATAL, msg(TX_ERR_MUTEX_PRINT_UNLOCK, 0), 1);
	return ((void *)p);
}
#endif

#if PRINT_MODE == PRINT_SUBMISSION

void	*philo_ret(t_philo *p, int code)
{
	die_and_drop_forks(p, 0);
	(void)code;
	return ((void *)p);
}
#endif
