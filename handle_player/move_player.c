#include "./cub3d.h"

double	deg2rad(double degree);
double	adjust_degree(double base_degree, double offset_degree);

void    move_player(t_game *game)
{
	unsigned int const	pressed_keyset = game->pressed_keyset;
	double const		angle = game->player.camera_angle;
	double const		move_speed = game->player.move_speed;
	t_vector2			*player_pos;
	double				sign;

	player_pos = &game->player.vec_pos;
	sign = 1.0;
	if (!!(pressed_keyset & KEYSET_W) ^ !!(pressed_keyset & KEYSET_S))
	{
		if (pressed_keyset & KEYSET_S)
			sign = -1.0;
		player_pos->x += move_speed * cos(deg2rad(angle)) * sign;
		player_pos->y += move_speed * sin(deg2rad(angle)) * sign;
	}
	if (!!(pressed_keyset & KEYSET_A) ^ !!(pressed_keyset & KEYSET_D))
	{
		if (pressed_keyset & KEYSET_A)
			sign = -1.0;
		player_pos->x += move_speed \
							* cos(deg2rad(adjust_degree(angle, 90.0 * sign)));
		player_pos->y += move_speed \
							* sin(deg2rad(adjust_degree(angle, 90.0 * sign)));		
	}
}