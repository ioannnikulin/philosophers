/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoui.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 18:34:04 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/25 18:30:21 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

unsigned int	atoui(const char *nptr, int *errno)
{
	unsigned long	res;
	size_t			cur;
	int				smth;

	res = 0;
	cur = 0;
	smth = 0;
	while (nptr[cur] && (nptr[cur] > 47 && nptr[cur] < 58))
	{
		res = res * 10 + nptr[cur ++] - '0';
		if (res > UINT_MAX)
		{
			*errno = 1;
			return (0);
		}
		smth = 1;
	}
	if (nptr[cur] || smth == 0)
	{
		*errno = 2;
		return (0);
	}
	return (res);
}