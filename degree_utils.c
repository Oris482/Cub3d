/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   degree_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaesjeon <jaesjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:57:27 by jaesjeon          #+#    #+#             */
/*   Updated: 2022/11/03 11:57:28 by jaesjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

double	deg2rad(double degree)
{
	return ((degree) * M_PI / 180.0);
}

double	adjust_degree(double base_degree, double offset_degree)
{
	double	ret_degree;

	ret_degree = base_degree + offset_degree;
	if (ret_degree >= 360.0)
		return (ret_degree - 360.0);
	if (ret_degree < 0)
		return (ret_degree + 360.0);
	return (ret_degree);
}
