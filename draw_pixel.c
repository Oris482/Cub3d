/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_pixel.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaemjeon <jaemjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:57:42 by jaesjeon          #+#    #+#             */
/*   Updated: 2022/11/13 10:03:01 by jaemjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	put_pixel_floor(t_game *game, int idx_x)
{
	t_img_data *const	view_data = &game->view_data;
	double const		color = game->floor_color;
	char				*dst;
	double				gradiant;
	int					idx_y;

	idx_y = game->wall_pixel[idx_x].y;
	gradiant = 0.4 + (0.6 - game->ray_data[idx_x].wall_distance * 0.05);
	set_range_double(&gradiant, 0, 1);
	while (idx_y < game->info.screen_y)
	{
		dst = view_data->addr + (idx_y * view_data->line_length + \
							idx_x * (view_data->bits_per_pixel / 8));
		gradiant += (1 - gradiant) * 10 / (game->info.screen_y - idx_y);
		*(unsigned int *)dst = create_trgb(game->player.view_trans, \
											get_r(color) * gradiant, \
											get_g(color) * gradiant, \
											get_b(color) * gradiant);
		idx_y++;
	}
}

void	put_pixel_ceiling(t_game *game, int idx_x)
{
	t_img_data *const	view_data = &game->view_data;
	t_vector2_d *const	wall_pixel = &game->wall_pixel[idx_x];
	char				*dst;
	int					idx_y;

	idx_y = 0;
	while (idx_y < wall_pixel->x)
	{
		dst = view_data->addr + (idx_y * view_data->line_length + \
							idx_x * (view_data->bits_per_pixel / 8));
		*(unsigned int *)dst = game->ceiling_color;
		idx_y++;
	}
}

t_vector2	get_wall_pixel(t_game *game, t_vector2 *wall_line)
{
	t_vector2	ret_vec;

	if (wall_line->x < 0)
		ret_vec.x = 0;
	else if (wall_line->x > game->info.screen_y)
		ret_vec.x = game->info.screen_y;
	else
		ret_vec.x = wall_line->x;
	if (wall_line->y > game->info.screen_y)
		ret_vec.y = game->info.screen_y;
	else if (wall_line->y < 0)
		wall_line->y = 0;
	else
		ret_vec.y = wall_line->y;
	return (ret_vec);
}

void	draw_line(t_game *game, int idx_x)
{
	put_pixel_ceiling(game, idx_x);
	put_pixel_wall(game, idx_x);
	put_pixel_floor(game, idx_x);
}
