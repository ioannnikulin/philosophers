/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:27:10 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/31 15:52:04 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

int	assign(int *to, int val, int ret)
{
	if (to)
		*to = val;
	return (ret);
}

int	mbzero(void *f, int bytes, int ret)
{
	char	*s;

	if (!f || bytes <= 0)
		return (ret);
	s = f;
	while (-- bytes > 0)
		s[bytes] = 0;
	return (ret);
}

int	mfree(int choice, void **w, int sz, int ret)
{
	if (!choice)
		return (ret);
	mbzero(*w, sz, ret);
	free(*w);
	return (0);
}

void	*mcalloc(size_t sz)
{
	void	*res;

	res = malloc(sz);
	if (!res)
		return (0);
	mbzero(res, sz, 0);
	return (res);
}
