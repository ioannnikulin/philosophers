/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_description.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 12:47:33 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/22 16:52:49 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

char	*state_description(int state)
{
	if (state == NEWBORN)
		return (TX_NEWBORN);
	if (state == TAKES)
		return (TX_TAKES);
	if (state == EATS)
		return (TX_EATS);
	if (state == SLEEPS)
		return (TX_SLEEPS);
	if (state == THINKS)
		return (TX_THINKS);
	if (state == DIES)
		return (TX_DIES);
	if (state == ENOUGH)
		return (TX_ENOUGH);
	if (state == LOOKS)
		return (TX_LOOKS);
	return ("ERROR");
}
