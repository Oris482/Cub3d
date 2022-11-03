/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_ceiling_floor_image.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaesjeon <jaesjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:58:37 by jaesjeon          #+#    #+#             */
/*   Updated: 2022/11/03 11:58:47 by jaesjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "mlx.h"
#include <stdio.h>

static void	_pixel_put_to_img(t_img_data *view_img, int color, \
					t_outside_wall_info outside_wall, double base_transperency)
{
	char	*dst;
	int		step;
	int		sign;
	int		y;
	double	gradiant;

	step = outside_wall.range_y[END] - outside_wall.range_y[START];
	sign = 1;
	if (step < 0)
	{
		sign = -1;
		step *= -1;
	}
	gradiant = base_transperency;
	y = outside_wall.range_y[START];
	while (y != outside_wall.range_y[END])
	{
		gradiant += (1.0 - base_transperency) / step;
		dst = view_img->addr + (y * view_img->line_length + \
						outside_wall.x * (view_img->bits_per_pixel / 8));
		*(unsigned int *)dst = create_trgb(0, get_r(color) * gradiant, \
							get_g(color) * gradiant, get_b(color) * gradiant);
		y += sign;
	}
}

void	set_range(int arg[2], int arg_start, int arg_end)
{
	arg[START] = arg_start;
	arg[END] = arg_end;
}

void	draw_ceiling_floor(t_game *game, int x, int wall_y[2], double fog_value)
{
	const int			screen_y = game->info.screen_y;
	t_outside_wall_info	outside_wall;

	outside_wall.x = x;
	if (wall_y[START] > 0)
	{
		set_range(outside_wall.range_y, wall_y[START], 0);
		_pixel_put_to_img(&game->view_data, game->ceiling_color, \
										outside_wall, fog_value);
	}
	if (wall_y[END] < screen_y)
	{
		set_range(outside_wall.range_y, wall_y[END], screen_y);
		_pixel_put_to_img(&game->view_data, game->floor_color, \
										outside_wall, fog_value);
	}
}
