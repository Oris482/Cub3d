/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_player.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaesjeon <jaesjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 13:54:45 by jaesjeon          #+#    #+#             */
/*   Updated: 2022/10/15 13:58:31 by jaesjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	cut_point(double num, int limiter)
{
	const int	handler = pow(10, limiter);
	int			sign;

	if (num < 0)
	{
		num *= -1.0;
		sign = -1;
	}
	else
		sign = 1;
	return ((double)((int)(num * handler)) / handler * sign);
}

void	check_wall_collisions(t_vector2 *player_pos, t_vector2 tmp_pos, \
															t_minimap *minimap)
{
	int					x[2];
	int					y[2];
	const int			wall_color = create_trgb(0, 0, 0, 0);
	const t_img_data	*img_data = &minimap->map_img_data;
	char				*dst[4];

	x[0] = (int)(tmp_pos.x * minimap->pixel_per_square) + 2;
	x[1] = (int)((tmp_pos.x + 1) * minimap->pixel_per_square) - 2;
	y[0] = (int)(tmp_pos.y * minimap->pixel_per_square) + 2;
	y[1] = (int)((tmp_pos.y + 1) * minimap->pixel_per_square) - 2;
	dst[0] = img_data->addr + (y[0] * img_data->line_length + \
										x[0] * (img_data->bits_per_pixel / 8));
	dst[1] = img_data->addr + (y[0] * img_data->line_length + \
										x[1] * (img_data->bits_per_pixel / 8));
	dst[2] = img_data->addr + (y[1] * img_data->line_length + \
										x[0] * (img_data->bits_per_pixel / 8));
	dst[3] = img_data->addr + (y[1] * img_data->line_length + \
										x[1] * (img_data->bits_per_pixel / 8));
	if (*dst[0] != wall_color && *dst[1] != wall_color && *dst[2] != wall_color \
															&& *dst[3] != wall_color)
	{
		player_pos->x = tmp_pos.x;
		player_pos->y = tmp_pos.y;
	}	
}

void    move_player(t_player *player, t_minimap *minimap, unsigned int const pressed_keyset)
{
	double const		angle = player->camera_angle;
	double const		move_speed = player->move_speed;
	t_vector2			tmp_pos;
	double				sign;

	tmp_pos = player->vec_pos;
	if (!!(pressed_keyset & KEYSET_W) ^ !!(pressed_keyset & KEYSET_S))
	{
		sign = 1.0;
		if (pressed_keyset & KEYSET_S)
			sign = -1.0;
		tmp_pos.x += move_speed * cut_point(cos(deg2rad(angle)), 6) * sign;
		tmp_pos.y += move_speed * cut_point(sin(deg2rad(angle)), 6) * sign;
	}
	if (!!(pressed_keyset & KEYSET_A) ^ !!(pressed_keyset & KEYSET_D))
	{
		sign = 1.0;
		if (pressed_keyset & KEYSET_A)
			sign = -1.0;
		tmp_pos.x += move_speed \
			* cut_point(cos(deg2rad(adjust_degree(angle, 90.0 * sign))), 6);
		tmp_pos.y += move_speed \
			* cut_point(sin(deg2rad(adjust_degree(angle, 90.0 * sign))), 6);		
	}
	check_wall_collisions(&player->vec_pos, tmp_pos, minimap);
}

void    rotate_player(t_player *player, unsigned int const pressed_keyset)
{
	double const		rotate_speed = player->rotate_speed;
	double				*angle;

	angle = &player->camera_angle;
	if (pressed_keyset & KEYSET_LA && !(pressed_keyset & KEYSET_RA))
		*angle = adjust_degree(*angle, -rotate_speed);
	if (pressed_keyset & KEYSET_RA && !(pressed_keyset & KEYSET_LA))
		*angle = adjust_degree(*angle, rotate_speed);
}
