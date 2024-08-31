/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 16:21:26 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/31 14:06:20 by inikulin         ###   ########.fr       */
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

static int	ull(unsigned long long int i, int sign)
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

#if PRINT_MODE == PRINT_FULL
void	print(long long int printed, long long int happened, t_philo *p, int state)
{
	char	s[26];
	int		i;

	i = -1;
	while (++i < 25)
		s[i] = ' ';
	s[25] = 0;
	prints(&s[printlli(printed, 15)], 0);
	prints(&s[printlli(happened, 15)], 0);
	prints(&s[printlli(p->i, 20)], 0);
	prints(&s[prints(state_description(state), 0)], 0);
	printlli(p->times_eaten, 0);
	prints("\n", 0);
}
#endif
#if PRINT_MODE == PRINT_SUBMISSION
void	print(long long int printed, long long int happened, t_philo *p, int state)
{
	(void)printed;
	printlli(happened / 1000, 0);
	prints(" ", 0);
	printlli(p->i, 0);
	prints(" ", 0);
	prints(state_description(state), 0);
	prints("\n", 0);
}
#endif
