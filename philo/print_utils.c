/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 16:21:26 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/31 17:44:30 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

int	prints(char *s, int ret)
{
	int	l;

	l = 0;
	while (s[l])
		l ++;
	write(1, s, l);
	return (ret + l);
}

int	ull(unsigned long long int i, int sign)
{
	char	digs[20];
	int		dig_idx;

	dig_idx = 19;
	while (i)
	{
		digs[dig_idx --] = i % 10 + '0';
		i /= 10;
	}
	if (sign < 0)
		digs[dig_idx --] = '-';
	write(1, &digs[dig_idx + 1], 19 - dig_idx);
	return (19 - dig_idx);
}

int	printull(unsigned long long i, int ret)
{
	if (!i)
	{
		write(1, "0", 1);
		return (ret + 1);
	}
	return (ret + ull(i, 1));
}

int	printlli(long long int i, int ret)
{
	int	sign;

	if (!i)
	{
		write(1, "0", 1);
		return (ret + 1);
	}
	if (i == -9223372036854775807)
	{
		write(1, "-9223372036854775807", 20);
		return (ret + 20);
	}
	sign = 1;
	if (i < 0)
	{
		sign = -1;
		i = -i;
	}
	return (ret + ull(i, sign));
}
