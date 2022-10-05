#include "./cub3d.h"

double	deg2rad(double degree);
double	adjust_degree(double base_degree, double offset_degree);

void    move_player(t_game *game)
{
	unsigned int const	pressed_keyset = game->pressed_keyset;
	double const		angle = game->player.camera_angle;
	double const		move_speed = game->player.move_speed;
	t_vector2			*player_pos;

	player_pos = &game->player.vec_pos;
	if (pressed_keyset & KEYSET_W && !(pressed_keyset & KEYSET_S))
	{
		player_pos->x += move_speed * cos(deg2rad(angle));
		player_pos->y += move_speed * sin(deg2rad(angle));
	}
	if (pressed_keyset & KEYSET_S && !(pressed_keyset & KEYSET_W))
	{
		player_pos->x -= move_speed * cos(deg2rad(angle));
		player_pos->y -= move_speed * sin(deg2rad(angle));
	}
	if (pressed_keyset & KEYSET_A && !(pressed_keyset & KEYSET_D))
	{
		player_pos->x += move_speed * cos(deg2rad(adjust_degree(angle, -90.0)));
		player_pos->y += move_speed * sin(deg2rad(adjust_degree(angle, -90.0)));		
	}
	if (pressed_keyset & KEYSET_D && !(pressed_keyset & KEYSET_A))
	{
		player_pos->x += move_speed * cos(deg2rad(adjust_degree(angle, +90.0)));
		player_pos->y += move_speed * sin(deg2rad(adjust_degree(angle, +90.0)));		
	}
}