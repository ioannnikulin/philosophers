/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:24:02 by inikulin          #+#    #+#             */
/*   Updated: 2024/06/15 20:24:52 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

int	m_init(t_mutex *m)
{
	return (pthread_mutex_init(m, 0));
}

int	m_kill(t_mutex *m)
{
	return (pthread_mutex_destroy(m));
}

int	m_lock(t_mutex *m)
{
	return (pthread_mutex_lock(m));
}

int	m_unlock(t_mutex *m)
{
	return (pthread_mutex_lock(m));
}
