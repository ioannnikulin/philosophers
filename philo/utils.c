/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:27:10 by inikulin          #+#    #+#             */
/*   Updated: 2024/05/26 20:24:54 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

int finalize(t_props *p, char *msg, int ret)
{
	int	i;

	if (*p)
	{
		free(p->philos);
		p->philos = 0;
		free(p->threads);
		p->threads = 0;
		i = -1;
		if (p->forks)
		{
			while (++ i < p->sz)
			{
				if (pthread_mutex_destroy(p->forks[i]))
					printf("Failed to destroy a mutex\n");
			}
			free(p->forks);
			p->forks = 0;
		}
		*p = 0;
	}
	if (*msg)
		printf("%s\n", msg);
	return (ret);
}

int	done(t_props *p, int print_unlock, char *msg, int ret)
{
	if (*p)
		p->enough = 1;
	if (*msg)
		printf("%s\n", msg);
	if (print_unlock && pthread_mutex_unlock(p->print_poll))
		return (done(0, 0, "Failed to unlock the print mutex", ret));
	return (ret);
}

time_t	time(time_t *t, int *ok)
{
	struct timeval	tv;

	if (gettimeofday(&tv, 0))
	{
		*ok = 0;
		return (0);
	}
	if (!t)
		return (tv.tv_sec * 1000000 + tv.tv_tvusec);
	return (tv.tv_sec * 1000000 + tv.tv_tvusec - *t);
}
