#include "../cub3d.h"
#include "mlx.h"
#include <unistd.h>

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

	// side_dist->x = fabs((trunc(pos->x + 1) - pos->x) / cos(radian_cast_angle));
	// side_dist->y = fabs((trunc(pos->y + 1) - pos->y) / sin(radian_cast_angle));
	delta_dist->x = 1 / fabs(cos(radian_cast_angle));
	delta_dist->y = 1 / fabs(sin(radian_cast_angle));
	side_dist->x = (fabs(floor(pos->x) - pos->x) ) * delta_dist->x;
	side_dist->y = (fabs(floor(pos->y) - pos->y) ) * delta_dist->y;
	// side_dist->x = (ceil(pos->x) - pos->x) * delta_dist->x;
	// side_dist->y = (ceil(pos->y) - pos->y) * delta_dist->y;
	// side_dist->x = (ceil(pos->x) - pos->x) / fabs(cos(radian_cast_angle));
	// side_dist->y = (ceil(pos->y) - pos->y) / fabs(sin(radian_cast_angle));
}

void	get_hit_wall_side(t_ray *ray, int last_step)
{
	if (last_step == 0) // x축방향으로 이동 -> 동쪽아니면 서쪽에 맞음
	{
		if (ray->cast_angle < 90 || 270 <= ray->cast_angle)
			ray->hit_wall_side = 1;
		else
			ray->hit_wall_side = 0;
	}
	else // y축방향으로 이동 -> 남쪽아니면 북쪽에 맞음
	{
		if (ray->cast_angle < 180)
			ray->hit_wall_side = 3;
		else
			ray->hit_wall_side = 2;
	}
	// printf("%d", ray->hit_wall_side);
}

int	ray_travel(t_ray *ray, t_vector2 *step, \
									t_vector2 *side_dist, t_vector2 *delta_dist)
{
	int		last_step;

	// if (side_dist->x < side_dist->y + delta_dist->y)
	// {
	// 	side_dist->x += delta_dist->x;
	// 	ray->hit_idx_x += step->x;
	// 	last_step = 0;
	// }
	// else
	// {
	// 	side_dist->y += delta_dist->y;
	// 	ray->hit_idx_y += step->y;
	// 	last_step = 1;
	// }
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

void	calcul_hitpoint_dist(t_player *player_info, t_ray *ray)
{
	const double	delta_x = player_info->vec_pos.x - ray->hit_point.x;
	const double	delta_y = player_info->vec_pos.y - ray->hit_point.y;
	double			delta_angle;

	delta_angle = fabs(player_info->camera_angle - ray->cast_angle);
	// while (delta_angle >= 360)
	// 	delta_angle -= 360;
	ray->ray_length = sqrt(pow(delta_x, 2) + pow(delta_y, 2));
	ray->wall_distance = fabs(cos(degree_to_radian(delta_angle))) * ray->ray_length;
	// ray->wall_distance = ray->ray_length;
}

void	calcul_texture_point(t_vector2 *cast_point, t_ray *ray)
{

}

void	set_ray_hit_point(t_vector2 *vec_pos, t_ray *ray)
{
	if (ray->hit_wall_side == 0)
	{
		ray->hit_point.x = ray->hit_idx_x + 1;
		ray->hit_point.y = \
			vec_pos->y + (vec_pos->x - ray->hit_point.x) * tan(degree_to_radian(ray->cast_angle));
	}
	else if (ray->hit_wall_side == 1)
	{
		ray->hit_point.x = ray->hit_idx_x;
		ray->hit_point.y = \
			vec_pos->y + (ray->hit_point.x - vec_pos->x) * tan(degree_to_radian(ray->cast_angle));
	}
	else if (ray->hit_wall_side == 2)
	{
		ray->hit_point.y = ray->hit_idx_y + 1;
		ray->hit_point.x = \
			vec_pos->x - (vec_pos->y - ray->hit_point.y) * tan(degree_to_radian(ray->cast_angle));
	}
	else
	{
		ray->hit_point.y = ray->hit_idx_y;
		ray->hit_point.x = \
			vec_pos->x + (ray->hit_point.y - vec_pos->y) * tan(degree_to_radian(ray->cast_angle));
	}
	// printf("wall side : %d\n", ray->hit_wall_side);
	// printf("hit_point x : %lf y : %lf\n", ray->hit_point.x, ray->hit_point.y);
	// if (ray->hit_point.x < 0 || ray->hit_point.y < 0)
	// {
	// 	printf("vecpos x:%lf  y:%lf\n", vec_pos->x, vec_pos->y);
	// 	printf("%d %d\n", ray->hit_idx_x, ray->hit_idx_y);
	// 	printf("cast_angle : %lf\n", ray->cast_angle);
	// 	printf("sin(cast_angle) = %lf\n", sin(ray->cast_angle));
	// 	// printf("%lf\n", vec_pos->y + (ray->hit_point.x - vec_pos->x) * (-0.5));
	// 	sleep(1);
	// }
}

void	new_calcul_hitpoint_dist(t_ray *ray, int last_step, \
									t_vector2 *side_dist, t_vector2 *delta_dist)
{
	if (last_step == 0) // x축을 마지막으로 이동
		ray->ray_length = side_dist->x - delta_dist->x;
	else
		ray->ray_length = side_dist->y - delta_dist->y;
}

void	reflect_hit_point(t_vector2 *cast_pos, t_ray *ray, double abs_delta_len)
{
	// if (ray->hit_wall_side == 0)
	// {
	// 	if (90 < ray->cast_angle && ray->cast_angle <= 180)
	// 		ray->hit_point.y = cast_pos->y + abs_delta_len + 1;
	// 	else
	// 		ray->hit_point.y = cast_pos->y + abs_delta_len;
	// }
	// else if (ray->hit_wall_side == 1)
	// {
	// 	if (ray->cast_angle <= 90)
	// 		ray->hit_point.y = cast_pos->y + abs_delta_len + 1;
	// 	else
	// 		ray->hit_point.y = cast_pos->y + abs_delta_len;
	// }
	// else if (ray->hit_wall_side == 2)
	// {
	// 	if (ray->cast_angle < 270)
	// 		ray->hit_point.x = cast_pos->x + abs_delta_len;
	// 	else
	// 		ray->hit_point.x = cast_pos->x + abs_delta_len + 1;
	// }
	// else
	// {
	// 	if (ray->cast_angle < 90)
	// 		ray->hit_point.x = cast_pos->x + abs_delta_len + 1;
	// 	else
	// 		ray->hit_point.x = cast_pos->x + abs_delta_len;
	// }
	if (ray->hit_wall_side == 2 || ray->hit_wall_side == 3)
	{
		if (cos(degree_to_radian(ray->cast_angle)) > 0)
			ray->hit_point.x = cast_pos->x + abs_delta_len;
		else
			ray->hit_point.x = cast_pos->x - abs_delta_len;
	}
	else
	{
		if (sin(degree_to_radian(ray->cast_angle)) > 0)
			ray->hit_point.y = cast_pos->y + abs_delta_len;
		else
			ray->hit_point.y = cast_pos->y - abs_delta_len;
	}
}

void	new_set_ray_hit_point(t_vector2 *cast_pos, t_ray *ray)
{
	double	abs_delta_len;

	if (ray->hit_wall_side == 0)
	{
		ray->hit_point.x = ray->hit_idx_x + 1;
		abs_delta_len = sqrt(pow(ray->ray_length, 2) - pow(cast_pos->x - ray->hit_point.x, 2));
	}
	else if (ray->hit_wall_side == 1)
	{
		ray->hit_point.x = ray->hit_idx_x;
		abs_delta_len = sqrt(pow(ray->ray_length, 2) - pow(ray->hit_point.x - cast_pos->x, 2));
	}
	else if (ray->hit_wall_side == 2)
	{
		ray->hit_point.y = ray->hit_idx_y + 1;
		abs_delta_len = sqrt(pow(ray->ray_length, 2) - pow(cast_pos->y - ray->hit_point.y, 2));
	}
	else
	{
		ray->hit_point.y = ray->hit_idx_y;
		abs_delta_len = sqrt(pow(ray->ray_length, 2) - pow(ray->hit_point.y -cast_pos->y, 2));
	}
	reflect_hit_point(cast_pos, ray, abs_delta_len);
}

void	new_set_wall_distance(t_player *player_info, t_ray *ray)
{
	double			delta_angle;

	delta_angle = fabs(player_info->camera_angle - ray->cast_angle);
	ray->wall_distance = cos(degree_to_radian(delta_angle)) * ray->ray_length;
	// printf("%lf\n", ray->ray_length);
	// ray->wall_distance = ray->ray_length;
}

void	cast_sigle_ray(t_game *game, t_ray *ray)
{
	t_vector2	step;
	t_vector2	side_dist;
	t_vector2	delta_dist;
	int			last_step;
	// static int			debug_i = 0;

	set_ray_step(&game->player.vec_pos, ray, &step);
	set_dist(&game->player.vec_pos, ray->cast_angle, &side_dist, &delta_dist);
	// printf("idx : %d side_dist (%lf %lf), delta_dist (%lf %lf)\n", debug_i, side_dist.x, side_dist.y, delta_dist.x, delta_dist.y);
	// debug_i++;
	// if (debug_i == game->info.screen_x)
	// 	exit(0);
	while (TRUE)
	{
		last_step = ray_travel(ray, &step, &side_dist, &delta_dist);
		if (game->info.map[ray->hit_idx_y][ray->hit_idx_x] == '1')
			break ;
	}
	get_hit_wall_side(ray, last_step);
	new_calcul_hitpoint_dist(ray, last_step, &side_dist, &delta_dist);
	new_set_ray_hit_point(&game->player.vec_pos, ray);
	new_set_wall_distance(&game->player, ray);
	// set_ray_hit_point(&game->player.vec_pos, ray);
	// printf("hit_x : %lf hit_y : %lf\n", ray->hit_point.x, ray->hit_point.y);
	// calcul_hitpoint_dist(&game->player, ray, &side_dist, &delta_dist);
	calcul_texture_point(&game->player.vec_pos, ray);
}

// void	set_cast_angle(t_ray, int idx_x)
// {
// 	double			cast_angle;

// 	cast_angle = game->player.camera_angle - (game->info.fov_h / 2) + delta_angle * idx_x;
// 	if (cast_angle < 0)
// 		cast_angle += 360;
// 	else if (cast_angle >= 360)
// 		cast_angle -= 360;
// 	game->ray_data[idx_x].cast_angle = cast_angle;
// }

void	debug_print_ray_info(t_game *game)
{
	int	i = 0;
	t_ray	*ray;

	while (i < game->info.screen_x)
	{
		ray = &game->ray_data[i];
		printf("index : %d cast_angle : %lf, ray_length : %lf, wall_dist : %lf, hit_point(%lf %lf)", \
		i, ray->cast_angle, ray->ray_length, ray->wall_distance, ray->hit_point.x, ray->hit_point.y);
		printf("hit_idx(%d %d), hit_side : %d\n", ray->hit_idx_x, ray->hit_idx_y, ray->hit_wall_side);
		i++;
	}
	printf("my pos (%lf %lf)\n", game->player.vec_pos.x, game->player.vec_pos.y);
	exit(0);
}

void	ray_cast(t_game *game)
{
	const double	delta_angle = game->info.fov_h / game->info.screen_x;
	double			cast_angle;
	int	idx_x;

	idx_x = 0;
	while (idx_x < game->info.screen_x)
	{
		cast_angle = game->player.camera_angle - \
								(game->info.fov_h / 2) + delta_angle * idx_x;
		game->ray_data[idx_x].cast_angle = cast_angle;
		cast_sigle_ray(game, &game->ray_data[idx_x]);
		// printf("%d :  x:%d y:%d\n", idx_x, game->ray_data[idx_x].hit_idx_x, game->ray_data[idx_x].hit_idx_y);
		idx_x++;
	}
	// debug_print_ray_info(game);
}

void	calcul_drawpixel(t_game *game, t_ray *ray, t_vector2 *wall_pixel)
{
	const double	ratio_wall = 1 / ray->wall_distance;
	// wall_pixel->x = ray->wall_distance * 50; // 그냥 대입해봄
	// wall_pixel->y = game->info.screen_y - ray->wall_distance * 50;

	wall_pixel->x = game->info.screen_y / 2 - ratio_wall * 100;
	wall_pixel->y = game->info.screen_y / 2 + ratio_wall * 100;
	if (wall_pixel->x < 0)
		wall_pixel->x = 0;
	if (wall_pixel->y > game->info.screen_y)
		wall_pixel->y = game->info.screen_y;
}

void	draw_line(t_game *game, int idx_x, t_vector2 *wall_pixel)
{
	t_bg_data	*bg_data;
	char		*dst;
	int			idx_y;

	idx_y = 0;
	bg_data = &game->bg_data;
	while (idx_y < wall_pixel->x)
	{
		dst = bg_data->addr + (idx_y * bg_data->line_length +
							   idx_x * (bg_data->bits_per_pixel / 8));
		*(unsigned int *)dst = create_trgb(0, 0, 0, 0);
		idx_y++;
	}
	while (idx_y < wall_pixel->y)
	{
		dst = bg_data->addr + (idx_y * bg_data->line_length +
							   idx_x * (bg_data->bits_per_pixel / 8));
		*(unsigned int *)dst = create_trgb(0, 100, 100, 100);
		idx_y++;
	}
	while (idx_y < game->info.screen_y)
	{
		dst = bg_data->addr + (idx_y * bg_data->line_length +
							   idx_x * (bg_data->bits_per_pixel / 8));
		*(unsigned int *)dst = create_trgb(0, 0, 0, 0);
		idx_y++;
	}
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
	// sleep(10);
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

void	reset_img(t_game *game)
{
	size_t	size_of_img;

	size_of_img = game->bg_data.line_length * game->info.screen_y + \
				game->info.screen_x * (game->bg_data.bits_per_pixel / 8);
	ft_memset(game->bg_data.addr, 0, sizeof(game->bg_data.img));
}

void	draw_screen(t_game *game)
{
	static int	frame = 0;
	t_vector2	wall_pixel;

	draw_wall(game, &wall_pixel);
	draw_back_ground(game, &wall_pixel);
	// 그리기
	// mlx_clear_window(game->info.mlx_ptr, game->info.win_ptr);
	mlx_put_image_to_window(game->info.mlx_ptr, game->info.win_ptr, game->bg_data.img, 0, 0);
	// reset_img(game);
	// printf("frame : %d\n", frame);
	frame++;
}
