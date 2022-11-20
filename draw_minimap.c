/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaesjeon <jaesjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:57:39 by jaesjeon          #+#    #+#             */
/*   Updated: 2022/11/13 13:35:22 by jaesjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "mlx.h"
#include <math.h>

void	put_one_square_pixels_to_screen(t_game *game, int x, int y, \
														unsigned int color)
{
	const int			init_x = x;
	const int			end_x = x + game->minimap.pixel_per_square;
	const int			end_y = y + game->minimap.pixel_per_square;
	const t_info		*info = &game->info;

	while (y < end_y)
	{
		x = init_x;
		while (x < end_x)
		{
			mlx_pixel_put(info->mlx_ptr, info->win_ptr, x, y, color);
			x++;
		}
		y++;
	}
}

void	draw_ray_line(double center[2], t_img_data *view_data, \
										double camera_angle_h, double ray_angle)
{
	const unsigned int	wall_color = create_trgb(0, 0, 0, 0);
	const unsigned int	player_color = create_trgb(0, 0, 0, 255);
	const unsigned int	ray_color = create_trgb(0, 255, 0, 0);
	char				*dst;
	double				pos[2];

	pos[X] = center[X];
	pos[Y] = center[Y];
	while (1)
	{
		dst = view_data->addr + ((int)pos[Y] * view_data->line_length + \
							(int)pos[X] * (view_data->bits_per_pixel / 8));
		if (*(unsigned int *)dst == wall_color)
			break ;
		else if (*(unsigned int *)dst != player_color)
			*(unsigned int *)dst = ray_color;
		pos[X] = pos[X] + cut_point(cos(deg2rad(camera_angle_h)), 6) + \
		cut_point(cos(deg2rad(adjust_degree(camera_angle_h, ray_angle))), 6);
		pos[Y] = pos[Y] + cut_point(sin(deg2rad(camera_angle_h)), 6) + \
		cut_point(sin(deg2rad(adjust_degree(camera_angle_h, ray_angle))), 6);
	}
}

void	draw_camera_angle_h(t_player *player, t_img_data *view_data, \
														int pps, double fov_h)
{
	const double	camera_angle_h = player->camera_angle_h;
	double			ray_center[2];
	double			ray_angle;
	const double	ray_interval = 4.0;

	ray_center[X] = (player->vec_pos.x - 0.5) * pps + pps / 2 - !(pps % 2);
	ray_center[Y] = (player->vec_pos.y - 0.5) * pps + pps / 2 - !(pps % 2);
	ray_angle = -fov_h;
	while (ray_angle < fov_h)
	{
		draw_ray_line(ray_center, view_data, camera_angle_h, ray_angle);
		ray_angle += ray_interval;
	}
	draw_ray_line(ray_center, view_data, camera_angle_h, fov_h);
}

void	draw_player(t_player *player, t_img_data *view_data, int pps)
{
	const int	player_x = (int)((player->vec_pos.x - 0.5) * pps);
	const int	player_y = (int)((player->vec_pos.y - 0.5) * pps);
	const int	player_color = create_trgb(0, 0, 0, 255);
	int			pos[2];

	set_range(pos, player_x, player_y);
	put_one_square_pixels_to_img(view_data, pos, pps, player_color);
}

void	draw_minimap(t_game *game)
{
	const int			pps = game->minimap.pixel_per_square;
	const t_img_data	*minimap_data = &game->minimap.map_img_data;
	t_img_data			*view_data;
	int					x;
	int					y;

	view_data = &game->view_data;
	y = 0;
	while (y < game->minimap.height)
	{
		x = 0;
		while (x < game->minimap.width)
		{
			*(unsigned int *)(view_data->addr + (y * view_data->line_length + \
				x * (view_data->bits_per_pixel / 8))) = \
					*(unsigned int *)(minimap_data->addr +
					(y * minimap_data->line_length + x \
						* (minimap_data->bits_per_pixel / 8)));
			x++;
		}
		y++;
	}
	draw_player(&game->player, &game->view_data, pps);
	draw_camera_angle_h(&game->player, &game->view_data, pps, game->info.fov_h);
}
