/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 16:21:26 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/26 13:02:29 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

#if PRINT_MODE == PRINT_FULL

void	print(long long int printed, long long int happened,
		t_philo *p, int state)
{
	char	s[26];
	int		i;

	i = -1;
	while (++i < 25)
		s[i] = ' ';
	s[25] = 0;
	prints(&s[printlli(printed, 15)], 0);
	prints(&s[printlli(happened, 15)], 0);
	prints(&s[printlli(p->i + 1, 20)], 0);
	prints(&s[prints(state_description(state), 0)], 0);
	printull(tsull_get_release(&p->times_eaten, &p->errno), 0);
	prints("\n", 0);
}
#endif

#if PRINT_MODE == PRINT_SUBMISSION

void	print(long long int printed, long long int happened,
		t_philo *p, int state)
{
	(void)printed;
	printlli(happened / 1000, 0);
	prints(" ", 0);
	printlli(p->i + 1, 0);
	prints(" ", 0);
	prints(state_description(state), 0);
	prints("\n", 0);
}
#endif
