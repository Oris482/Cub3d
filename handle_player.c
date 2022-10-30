#include "cub3d.h"
#include "mlx.h"

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

void	check_wall_collisions(t_vector2 *player_pos, t_vector2 *tmp_pos, char **map)
{
	t_vector2	check_coordin;

	if (tmp_pos->x - player_pos->x > 0)
		check_coordin.x = tmp_pos->x + BODY_SIDE_2;
	else
		check_coordin.x = tmp_pos->x - BODY_SIDE_2;
	if (tmp_pos->y - player_pos->y > 0)
		check_coordin.y = tmp_pos->y + BODY_SIDE_2;
	else
		check_coordin.y = tmp_pos->y - BODY_SIDE_2;
	if (map[(int)check_coordin.y][(int)player_pos->x] != WALL)
		player_pos->y = tmp_pos->y;
	if (map[(int)player_pos->y][(int)check_coordin.x] != WALL)
		player_pos->x = tmp_pos->x;
}

void    move_player(t_player *player, char **map, unsigned int const pressed_keyset)
{
	double const		angle = player->camera_angle_h;
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
	check_wall_collisions(&player->vec_pos, &tmp_pos, map);
}

void    rotate_player_key(t_player *player, unsigned int const pressed_keyset)
{
	double const		rotate_speed = player->rotate_speed;
	double				*angle;

	angle = &player->camera_angle_h;
	if (pressed_keyset & KEYSET_LA && !(pressed_keyset & KEYSET_RA))
		*angle = adjust_degree(*angle, -rotate_speed);
	if (pressed_keyset & KEYSET_RA && !(pressed_keyset & KEYSET_LA))
		*angle = adjust_degree(*angle, rotate_speed);
	if (pressed_keyset & KEYSET_DA && !(pressed_keyset & KEYSET_UA))
		player->vertical_dist_pixel += 10;
	if (pressed_keyset & KEYSET_UA && !(pressed_keyset & KEYSET_DA))
		player->vertical_dist_pixel -= 10;
}

void	rotate_player_mouse(t_game *game)
{
	t_vector2_d	delta_mousepos;
	double * const		angle_h = &game->player.camera_angle_h;
	double				new_camera_angle_h;
	double				new_vertical_dis_pixel;
	double				ratio_pixel2rad;

	mlx_mouse_get_pos(game->info.win_ptr, &delta_mousepos.x, &delta_mousepos.y);
	*angle_h = adjust_degree(*angle_h, \
				(delta_mousepos.x - game->info.screen_x / 2) * SPEED_MOUSE_H);
	new_vertical_dis_pixel = game->player.vertical_dist_pixel + \
				(delta_mousepos.y - game->info.screen_y / 2) * SPEED_MOUSE_V;
	if (new_vertical_dis_pixel < -game->info.screen_y)
		new_vertical_dis_pixel = -game->info.screen_y;
	else if (new_vertical_dis_pixel > game->info.screen_y)
		new_vertical_dis_pixel = game->info.screen_y;
	game->player.vertical_dist_pixel = new_vertical_dis_pixel;
	mlx_mouse_move(game->info.win_ptr, \
							game->info.screen_x / 2, game->info.screen_y / 2);
}