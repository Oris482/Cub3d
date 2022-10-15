#include "../cub3d.h"

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

void	set_ray_step(t_vector2 *vec_pos, t_ray *ray, t_vector2 *step)
{
	{
		if (ray->cast_angle < 90 || ray->cast_angle >= 270)
			step->x = 1;
		else
			step->x = -1;
		if (ray->cast_angle < 180)
			step->y = 1;
		else
			step->y = -1;
	}
	{
		ray->hit_idx_x = trunc(vec_pos->x);
		ray->hit_idx_y = trunc(vec_pos->y);
	}
}

void	set_dist(t_vector2 *pos, double cast_angle, \
								t_vector2 *side_dist, t_vector2 *delta_dist)
{
	const	radian_cast_angle = degree_to_radian(cast_angle);

	side_dist->x = abs((trunc(pos->x + 1) - pos->x) / cos(radian_cast_angle));
	side_dist->y = abs((trunc(pos->y + 1) - pos->y) / sin(radian_cast_angle));
	delta_dist->x = abs(1 / cos(radian_cast_angle));
	delta_dist->y = abs(1 / sin(radian_cast_angle));
}

void	cast_sigle_ray(t_vector2 *vec_pos, t_ray *ray, char **map, t_info *info)
{
	t_vector2	step;
	t_vector2	side_dist;
	t_vector2	delta_dist;
	int			last_step;

	set_ray_step(vec_pos, ray, &step);
	set_dist(vec_pos, ray->cast_angle, &side_dist, &delta_dist);
	while (TRUE)
	{
		if (side_dist.x < side_dist.y)
		{
			side_dist.x += delta_dist.x;
			ray->hit_idx_x += step.x;
			last_step = 0;
		}
		else
		{
			side_dist.y += delta_dist.y;
			ray->hit_idx_y += step.y;
			last_step = 1;
		}
		if (map[ray->hit_idx_y][ray->hit_idx_x] == 1)
			break ;
	}
	// 벽까지의 거리, 텍스처좌표를 가져와야함.
}

void	ray_cast(t_game *game)
{
	int	idx_x;

	idx_x = 0;
	while (idx_x < game->info.screen_x)
	{
		cast_sigle_ray(&game->player.vec_pos, game->ray_data, \
														game->map, &game->info);
		idx_x++;
	}
}

void	draw_wall(t_game *game)
{
	ray_cast(game);
}
