/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calcul_ray.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaemjeon <jaemjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:59:53 by jaemjeon          #+#    #+#             */
/*   Updated: 2022/11/02 16:00:38 by jaemjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	calcul_texture_x_point(t_vector2 *cast_point, t_ray *ray)
{
	if (ray->hit_wall_side == EA)
		ray->hit_texture_point = ray->hit_idx_y + 1 - ray->hit_point.y;
	else if (ray->hit_wall_side == WE)
		ray->hit_texture_point = ray->hit_point.y - ray->hit_idx_y;
	else if (ray->hit_wall_side == SO)
		ray->hit_texture_point = ray->hit_point.x - ray->hit_idx_x;
	else
		ray->hit_texture_point = ray->hit_idx_x + 1 - ray->hit_point.x;
}

void	calcul_hitpoint_dist(t_ray *ray, int last_step, \
								t_vector2 *side_dist, t_vector2 *delta_dist)
{
	if (last_step == 0)
		ray->ray_length = side_dist->x - delta_dist->x;
	else
		ray->ray_length = side_dist->y - delta_dist->y;
}

void	calcul_drawpixel(t_game *game, t_ray *ray, int idx_x)
{
	const double		ratio_wall = 5 / (ray->wall_distance * 6);
	t_vector2 *const	wall_line = &game->wall_line[idx_x];
	t_vector2_d *const	wall_pixel = &game->wall_pixel[idx_x];

	wall_line->x = game->info.screen_y / 2 \
					- ratio_wall * game->info.screen_y \
					- game->player.vertical_dist_pixel;
	wall_line->y = game->info.screen_y / 2 \
					+ ratio_wall * game->info.screen_y \
					- game->player.vertical_dist_pixel;
	wall_pixel->x = wall_line->x;
	wall_pixel->y = wall_line->y;
	set_range_int(&wall_pixel->x, 0, game->info.screen_y);
	set_range_int(&wall_pixel->y, 0, game->info.screen_y);
}

void	get_hit_wall_side(t_ray *ray, int last_step)
{
	if (last_step == 0)
	{
		if (ray->cast_angle < 90 || 270 <= ray->cast_angle)
			ray->hit_wall_side = WE;
		else
			ray->hit_wall_side = EA;
	}
	else
	{
		if (ray->cast_angle < 180)
			ray->hit_wall_side = NO;
		else
			ray->hit_wall_side = SO;
	}
}

int	ray_travel(t_ray *ray, t_vector2 *step, \
					t_vector2 *side_dist, t_vector2 *delta_dist)
{
	int		last_step;

	if (side_dist->y < side_dist->x)
	{
		side_dist->y += delta_dist->y;
		ray->hit_idx_y += step->y;
		last_step = 1;
	}
	else
	{
		side_dist->x += delta_dist->x;
		ray->hit_idx_x += step->x;
		last_step = 0;
	}
	return (last_step);
}
