/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_casting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaemjeon <jaemjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 16:00:01 by jaemjeon          #+#    #+#             */
/*   Updated: 2022/11/13 10:02:35 by jaemjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_ray(t_game *game)
{
	int				idx_x;
	const double	delta_angle = game->info.fov_h / game->info.screen_x;

	idx_x = 0;
	game->ray_data = (t_ray *)my_malloc(sizeof(t_ray) * game->info.screen_x);
	while (idx_x < game->info.screen_x)
	{
		game->ray_data[idx_x].cast_angle = \
							delta_angle * idx_x - (game->info.fov_h / 2);
		if (game->ray_data[idx_x].cast_angle < 0)
			game->ray_data[idx_x].cast_angle += 360;
		else if (game->ray_data[idx_x].cast_angle >= 360)
			game->ray_data[idx_x].cast_angle -= 360;
		idx_x++;
	}
}

void	cast_sigle_ray(t_game *game, t_ray *ray)
{
	t_vector2	step;
	t_vector2	side_dist;
	t_vector2	delta_dist;
	int			last_step;

	set_ray_step(&game->player.vec_pos, ray, &step);
	set_dist(&game->player.vec_pos, ray->cast_angle, &side_dist, &delta_dist);
	while (TRUE)
	{
		last_step = ray_travel(ray, &step, &side_dist, &delta_dist);
		if (game->info.map[ray->hit_idx_y][ray->hit_idx_x] == '1')
			break ;
	}
	get_hit_wall_side(ray, last_step);
	calcul_hitpoint_dist(ray, last_step, &side_dist, &delta_dist);
	set_ray_hit_point(&game->player.vec_pos, ray);
	set_wall_distance(&game->player, ray);
	calcul_texture_x_point(ray);
}

void	ray_cast(t_game *game)
{
	const double	delta_angle = game->info.fov_h / game->info.screen_x;
	double			cast_angle;
	int				idx_x;

	idx_x = 0;
	while (idx_x < game->info.screen_x)
	{
		cast_angle = game->player.camera_angle_h - \
								(game->info.fov_h / 2) + delta_angle * idx_x;
		if (cast_angle < 0)
			cast_angle += 360.0;
		else if (cast_angle >= 360.0)
			cast_angle -= 360.0;
		game->ray_data[idx_x].cast_angle = cast_angle;
		cast_sigle_ray(game, &game->ray_data[idx_x]);
		idx_x++;
	}
}

void	draw_screen(t_game *game)
{
	int	idx_x;

	ray_cast(game);
	idx_x = 0;
	while (idx_x < game->info.screen_x)
	{
		calcul_drawpixel(game, &game->ray_data[idx_x], idx_x);
		draw_line(game, idx_x);
		idx_x++;
	}
}
