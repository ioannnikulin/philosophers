/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_s_ull_w.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 21:13:13 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/25 21:14:39 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosop.h"

int	ret_errno(t_s_ull *errno, int val, int ret)
{
	tsull_set_release(errno, 0, val, 0);
	return (ret);
}