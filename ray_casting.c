#include "cub3d.h"
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
		ray->hit_idx_x = floor(vec_pos->x);
		ray->hit_idx_y = floor(vec_pos->y);
	}
}

void	set_dist(t_vector2 *pos, double cast_angle, \
								t_vector2 *side_dist, t_vector2 *delta_dist)
{
	const	double radian_cast_angle = deg2rad(cast_angle);

	delta_dist->x = 1 / fabs(cos(radian_cast_angle));
	delta_dist->y = 1 / fabs(sin(radian_cast_angle));
	if (cos(radian_cast_angle) >= 0)
		side_dist->x = delta_dist->x * (ceil(pos->x) - pos->x);
	else
		side_dist->x = delta_dist->x * (pos->x - floor(pos->x));
	if (sin(radian_cast_angle) >= 0)
		side_dist->y = delta_dist->y * (ceil(pos->y) - pos->y);
	else
		side_dist->y = delta_dist->y * (pos->y - floor(pos->y));
}

void	get_hit_wall_side(t_ray *ray, int last_step)
{
	if (last_step == 0) // x축방향으로 이동 -> 동쪽아니면 서쪽에 맞음
	{
		if (ray->cast_angle < 90 || 270 <= ray->cast_angle)
			ray->hit_wall_side = WE;
		else
			ray->hit_wall_side = EA;
	}
	else // y축방향으로 이동 -> 남쪽아니면 북쪽에 맞음
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

void	calcul_hitpoint_dist(t_player *player_info, t_ray *ray)
{
	const double	delta_x = player_info->vec_pos.x - ray->hit_point.x;
	const double	delta_y = player_info->vec_pos.y - ray->hit_point.y;
	double			delta_angle;

	delta_angle = fabs(player_info->camera_angle_h - ray->cast_angle);
	ray->ray_length = sqrt(pow(delta_x, 2) + pow(delta_y, 2));
	ray->wall_distance = fabs(cos(deg2rad(delta_angle))) * ray->ray_length;
}

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

void	set_ray_hit_point(t_vector2 *vec_pos, t_ray *ray)
{
	if (ray->hit_wall_side == EA)
	{
		ray->hit_point.x = ray->hit_idx_x + 1;
		ray->hit_point.y = \
			vec_pos->y + (vec_pos->x - ray->hit_point.x) * tan(deg2rad(ray->cast_angle));
	}
	else if (ray->hit_wall_side == WE)
	{
		ray->hit_point.x = ray->hit_idx_x;
		ray->hit_point.y = \
			vec_pos->y + (ray->hit_point.x - vec_pos->x) * tan(deg2rad(ray->cast_angle));
	}
	else if (ray->hit_wall_side == SO)
	{
		ray->hit_point.y = ray->hit_idx_y + 1;
		ray->hit_point.x = \
			vec_pos->x - (vec_pos->y - ray->hit_point.y) * tan(deg2rad(ray->cast_angle));
	}
	else
	{
		ray->hit_point.y = ray->hit_idx_y;
		ray->hit_point.x = \
			vec_pos->x + (ray->hit_point.y - vec_pos->y) * tan(deg2rad(ray->cast_angle));
	}
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
	if (ray->hit_wall_side == SO || ray->hit_wall_side == NO)
	{
		if (cos(deg2rad(ray->cast_angle)) > 0)
			ray->hit_point.x = cast_pos->x + abs_delta_len;
		else
			ray->hit_point.x = cast_pos->x - abs_delta_len;
	}
	else
	{
		if (sin(deg2rad(ray->cast_angle)) > 0)
			ray->hit_point.y = cast_pos->y + abs_delta_len;
		else
			ray->hit_point.y = cast_pos->y - abs_delta_len;
	}
}

void	new_set_ray_hit_point(t_vector2 *cast_pos, t_ray *ray)
{
	double	abs_delta_len;

	if (ray->hit_wall_side == EA)
	{
		ray->hit_point.x = ray->hit_idx_x + 1;
		abs_delta_len = sqrt(pow(ray->ray_length, 2) - pow(cast_pos->x - ray->hit_point.x, 2));
	}
	else if (ray->hit_wall_side == WE)
	{
		ray->hit_point.x = ray->hit_idx_x;
		abs_delta_len = sqrt(pow(ray->ray_length, 2) - pow(ray->hit_point.x - cast_pos->x, 2));
	}
	else if (ray->hit_wall_side == SO)
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

	delta_angle = fabs(player_info->camera_angle_h - ray->cast_angle);
	ray->wall_distance = cos(deg2rad(delta_angle)) * ray->ray_length;
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
	new_calcul_hitpoint_dist(ray, last_step, &side_dist, &delta_dist);
	new_set_ray_hit_point(&game->player.vec_pos, ray);
	new_set_wall_distance(&game->player, ray);
	calcul_texture_x_point(&game->player.vec_pos, ray);
}

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

void	calcul_drawpixel(t_game *game, int idx_x, t_vector2 *wall_line)
{
	const double	ratio_wall = 1 / game->ray_data[idx_x].wall_distance;

	wall_line->x = game->info.screen_y / 2 \
					- ratio_wall * game->info.screen_y \
					- game->player.vertical_dist_pixel;
	wall_line->y = game->info.screen_y / 2 \
					+ ratio_wall * game->info.screen_y \
					- game->player.vertical_dist_pixel;
	game->wall_pixel[idx_x].x = wall_line->x;
	game->wall_pixel[idx_x].y = wall_line->y;
	if (game->wall_pixel[idx_x].x < 0)
		game->wall_pixel[idx_x].x = 0;
	else if (game->wall_pixel[idx_x].x > game->info.screen_y)
		game->wall_pixel[idx_x].x = game->info.screen_y;
	if (game->wall_pixel[idx_x].y < 0)
		game->wall_pixel[idx_x].y = 0;
	else if (game->wall_pixel[idx_x].y > game->info.screen_y)
		game->wall_pixel[idx_x].y = game->info.screen_y;
}

// void	calcul_wall_pixel(t_game *game, t_vector2 *wall_line)
// {
// 	if (wall_line->x < 0)
// 		ret_vec.x = 0;
// 	else if (wall_line->x > game->info.screen_y)
// 		ret_vec.x = game->info.screen_y;
// 	else
// 		ret_vec.x = wall_line->x;
// 	if (wall_line->y > game->info.screen_y)
// 		ret_vec.y = game->info.screen_y;
// 	else if (wall_line->y < 0)
// 		wall_line->y = 0;
// 	else
// 		ret_vec.y = wall_line->y;
// 	return (ret_vec);
// }

void	put_pixel_ceiling(t_game *game, int	idx_x, t_vector2 *wall_line)
{
	t_img_data * const	view_data = &game->view_data;
	double				fade_distance = wall_line->x + game->info.screen_y;
	char				*dst;
	int					idx_y;

	idx_y = 0;
	while (idx_y < game->wall_pixel[idx_x].x)
	{
		dst = view_data->addr + (idx_y * view_data->line_length + \
							   idx_x * (view_data->bits_per_pixel / 8));
		*(unsigned int *)dst = game->ceiling_color;
		idx_y++;
	}
}

void	put_pixel_floor(t_game *game, int idx_x, t_vector2 *wall_line)
{
	t_img_data * const	view_data = &game->view_data;
	double const		color = game->floor_color;
	char				*dst;
	double				gradiant;
	int					idx_y;

	idx_y = game->wall_pixel[idx_x].y;
	// gradiant = 1 - game->ray_data[idx_x].wall_distance * 0.07;
	gradiant = 1 - game->ray_data[idx_x].ray_length * 0.07;
	if (gradiant > 1)
		gradiant = 1;
	else if (gradiant < 0)
		gradiant = 0;

	while (idx_y < game->info.screen_y)
	{
		dst = view_data->addr + (idx_y * view_data->line_length + \
							   idx_x * (view_data->bits_per_pixel / 8));
		// gradiant = 1 - \
		// 	((wall_pixel->y - idx_y) / (game->info.screen_y - wall_pixel->y));
		gradiant += (1 - gradiant) / (game->info.screen_y - idx_y);
		*(unsigned int *)dst = create_trgb(0, get_r(color) * gradiant, \
							get_g(color) * gradiant, get_b(color) * gradiant);
		idx_y++;
	}

	// idx_y = game->info.screen_y;
	// gradiant = 1;
	// while (idx_y >= wall_pixel->y)
	// {
	// 	dst = view_data->addr + (idx_y * view_data->line_length + \
	// 						   idx_x * (view_data->bits_per_pixel / 8));

	// 	*(unsigned int *)dst = create_trgb(0, get_r(color) * gradiant, \
	// 						get_g(color) * gradiant, get_b(color) * gradiant);
	// }
}

void	draw_line(t_game *game, int idx_x, t_vector2 *wall_line)
{
	char		*dst;
	int			idx_y;

	put_pixel_ceiling(game, idx_x, wall_line);
	put_pixel_wall(game, idx_x, wall_line);
	put_pixel_floor(game, idx_x, wall_line);
}

void	make_wall(t_game *game)
{
	t_vector2	wall_line;
	int	idx_x;

	idx_x = 0;
	while (idx_x < game->info.screen_x)
	{
		calcul_drawpixel(game, idx_x, &wall_line);
		draw_line(game, idx_x, &wall_line);
		idx_x++;
	}
}

void	make_pixels_fade(t_game *game, int idx_x, int idx_y)
{
	t_img_data * const	view_data = &game->view_data;
	int					end_idx[2];
	int					ij[2];
	int					idx_trgb;
	char				*dst;

	end_idx[X] = idx_x + FADE_BLOCK_SIZE;
	end_idx[Y] = idx_y + FADE_BLOCK_SIZE;
	if (end_idx[X] > game->info.screen_x)
		end_idx[X] = game->info.screen_x;
	if (end_idx[Y] > game->info.screen_y)
		end_idx[Y] = game->info.screen_y;
	ij[X] = 0;
	while (idx_x + ij[X] < end_idx[X])
	{
		ij[Y] = 0;
		while (idx_y + ij[Y] < end_idx[Y])
		{
			idx_trgb = 0;
			dst = view_data->addr + (idx_y * view_data->line_length +
									 idx_x * (view_data->bits_per_pixel) / 8);
			while (idx_trgb < 4)
			{
				game->mixed_trgb[idx_x * idx_y * 4 + idx_trgb] += *(dst + idx_trgb);
				idx_trgb++;
			}
			ij[Y]++;
		}
		ij[X]++;
	}
	idx_trgb = 0;
	while (idx_trgb < 4)
	{
		game->mixed_trgb[idx_x * idx_y * 4 + idx_trgb] /= (FADE_BLOCK_SIZE * FADE_BLOCK_SIZE);
		idx_trgb++;
	}
	ij[X] = 0;
	while (idx_x + ij[X] < end_idx[X])
	{
		ij[Y] = 0;
		while (idx_y + ij[Y] < end_idx[Y])
		{
			dst = view_data->addr + ((idx_y + ij[Y]) * view_data->line_length + \
					(idx_x + ij[X]) * (view_data->bits_per_pixel / 8));
			*(unsigned int *)dst = create_trgb(game->mixed_trgb[idx_x * idx_y * 4 + 3], \
			game->mixed_trgb[idx_x * idx_y * 4 + 2], game->mixed_trgb[idx_x * idx_y * 4 + 1], \
			game->mixed_trgb[idx_x * idx_y * 4]);
			ij[Y]++;
		}
		ij[X]++;
	}
}

void	make_floor_fade(t_game *game)
{
	int	idx_x;
	int	idx_y;

	ft_memset(game->mixed_trgb, 0, (game->info.screen_x / FADE_BLOCK_SIZE)\
			 * (game->info.screen_y / FADE_BLOCK_SIZE) * sizeof(int) * 4);
	idx_x = 0;
	while (idx_x < game->info.screen_x)
	{
		idx_y = game->wall_pixel[idx_x].y;
		while (idx_y < game->info.screen_y)
		{
			make_pixels_fade(game, idx_x, idx_y);
			idx_y += FADE_BLOCK_SIZE;
		}
		idx_x += FADE_BLOCK_SIZE;
	}
}

void	draw_screen(t_game *game)
{
	ray_cast(game);
	make_wall(game);
	// make_floor_fade(game);
}
