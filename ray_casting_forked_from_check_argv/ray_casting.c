#include "../cub3d.h"
#include "mlx.h"

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
		ray->hit_idx_x = (int)(vec_pos->x);
		ray->hit_idx_y = (int)(vec_pos->y);
	}
}

void	set_dist(t_vector2 *pos, double cast_angle, \
								t_vector2 *side_dist, t_vector2 *delta_dist)
{
	const	double radian_cast_angle = degree_to_radian(cast_angle);

	side_dist->x = fabs((trunc(pos->x + 1) - pos->x) / cos(radian_cast_angle));
	side_dist->y = fabs((trunc(pos->y + 1) - pos->y) / sin(radian_cast_angle));
	delta_dist->x = fabs(1 / cos(radian_cast_angle));
	delta_dist->y = fabs(1 / sin(radian_cast_angle));
}

void	get_hit_wall_side(t_ray *ray, int last_step)
{
	// if (last_step == 0)
}

int	ray_travel(t_ray *ray, t_vector2 *step, \
									t_vector2 *side_dist, t_vector2 *delta_dist)
{
	int	last_step;

	if (side_dist->x < side_dist->y)
	{
		side_dist->x += delta_dist->x;
		ray->hit_idx_x += step->x;
		last_step = 0;
	}
	else
	{
		side_dist->y += delta_dist->y;
		ray->hit_idx_y += step->y;
		last_step = 1;
	}
	return (last_step);
}

void	calcul_hitpoint_dist(t_vector2 *cast_point, t_ray *ray, int last_step)
{

}

void	calcul_texture_point(t_vector2 *cast_point, t_ray *ray)
{

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
		last_step = ray_travel(ray, &step, &side_dist, &delta_dist);
		printf("%c\n", map[ray->hit_idx_y][ray->hit_idx_x]);
		if (map[ray->hit_idx_y][ray->hit_idx_x] == '1')
			break ;
	}
	get_hit_wall_side(ray, last_step);
	calcul_hitpoint_dist(vec_pos, ray, last_step);
	calcul_texture_point(vec_pos, ray);
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

void	calcul_drawpixel(t_game *game, t_ray *ray, t_vector2 *wall_pixel)
{

	wall_pixel->x = game->info.screen_y / 2 - ray->ray_length / 2; // 그냥 대입해봄
	wall_pixel->y = game->info.screen_y / 2 + ray->ray_length / 2;
	if (wall_pixel->x < 0)
		wall_pixel->x = 0;
	if (wall_pixel->y > game->info.screen_y)
		wall_pixel->y = game->info.screen_y;
}

void	draw_line(t_game *game, int idx_x, t_vector2 *wall_pixel)
{
	t_bg_data	*bg_data;
	char		*dst;

	bg_data = &game->bg_data;
	dst = bg_data->addr + ((int)wall_pixel->y * bg_data->line_length + \
										idx_x * (bg_data->bits_per_pixel / 8));
	*(unsigned int *)dst = create_trgb(0, 126, 126, 126);
}

void	make_wall(t_game *game, t_vector2 *wall_pixel)
{
	int	idx_x;

	idx_x = 0;
	while (idx_x < game->info.screen_x)
	{
		calcul_drawpixel(game, &game->ray_data[idx_x], wall_pixel);
		draw_line(game, idx_x, wall_pixel);
		idx_x++;
	}
}

void	draw_wall(t_game *game, t_vector2 *wall_pixel)
{
	ray_cast(game);
	make_wall(game, wall_pixel);
}

void	draw_back_ground(t_game *game, t_vector2 *wall_pixel)
{
	//	천장이랑 바닥 채우기
}

void	draw_screen(t_game *game)
{
	t_vector2	wall_pixel;

	draw_wall(game, &wall_pixel);
	draw_back_ground(game, &wall_pixel);
	// 그리기
	mlx_put_image_to_window(game->info.mlx_ptr, game->info.win_ptr, game->bg_data.img, 0, 0);
}
