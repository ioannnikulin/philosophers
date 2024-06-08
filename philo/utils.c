/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:27:10 by inikulin          #+#    #+#             */
/*   Updated: 2024/06/08 17:02:11 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

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

int	assign(int *to, int val, int ret)
{
	if (to)
		*to = val;
	return (ret);
}

int	mbzero(void *f, int bytes, int ret)
{
	char	*s;

	if (!f || !bytes)
		return (ret);
	s = f;
	while (-- bytes >= 0)
		s[bytes] = 0;
	return (ret);
}

int	mfree(int choice, void **w, int sz, int ret)
{
	if (!choice)
		return (ret);
	free(*w);
	return (mbzero(*w, sz, ret));
}
