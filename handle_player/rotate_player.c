#include "./cub3d.h"

double	adjust_degree(double base_degree, double offset_degree);

void    rotate_player(t_game *game)
{
	unsigned int const	pressed_keyset = game->pressed_keyset;
	double const		rotate_speed = game->player.rotate_speed;
	double				*angle;

	angle = &game->player.camera_angle;
	if (pressed_keyset & KEYSET_LA && !(pressed_keyset & KEYSET_RA))
		*angle = adjust_degree(*angle, -rotate_speed);
	if (pressed_keyset & KEYSET_RA && !(pressed_keyset & KEYSET_LA))
		*angle = adjust_degree(*angle, rotate_speed);
}