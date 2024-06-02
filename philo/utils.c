/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:27:10 by inikulin          #+#    #+#             */
/*   Updated: 2024/06/02 18:49:44 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

int	finalize(t_props *p, char *msg, int ret)
{
	unsigned int	i;

	if (p)
	{
		free(p->philos);
		p->philos = 0;
		free(p->threads);
		p->threads = 0;
		i = 0;
		if (p->forks)
		{
			while (i < p->sz)
			{
				if (pthread_mutex_destroy(&p->forks[i ++]))
					printf("Failed to destroy a mutex\n");
			}
			free(p->forks);
			p->forks = 0;
		}
		free(p);
	}
	if (*msg)
		printf("%s\n", msg);
	return (ret);
}

int	done(t_props *p, int print_unlock, char *msg, int ret)
{
	if (p)
		p->enough = 1;
	if (msg)
		printf("%s\n", msg);
	if (print_unlock && pthread_mutex_unlock(&p->print_poll))
		return (done(0, 0, "Failed to unlock the print mutex", ret));
	return (ret);
}

t_usec	mtime(t_usec *t, int *errno)
{
	struct timeval	tv;

	if (errno && *errno)
		return (0);
	if (gettimeofday(&tv, 0))
	{
		if (errno)
			*errno = 1;
		printf("Timer error\n");
		return (0);
	}
	if (!t)
		return (tv.tv_sec * 1000000 + tv.tv_usec);
	return (tv.tv_sec * 1000000 + tv.tv_usec - *t);
}
