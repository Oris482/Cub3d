/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaesjeon <jaesjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:59:58 by jaemjeon          #+#    #+#             */
/*   Updated: 2022/11/03 11:59:59 by jaesjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	set_range_int(int *to_set, int floor, int ceil)
{
	if (*to_set < floor)
		*to_set = floor;
	else if (*to_set > ceil)
		*to_set = ceil;
}

void	set_range_double(double *to_set, double floor, double ceil)
{
	if (*to_set < floor)
		*to_set = floor;
	else if (*to_set > ceil)
		*to_set = ceil;
}

int	exit_game(int exit_status)
{
	exit(exit_status);
}
