#include "./cub3d.h"

#define UNDEFINED	-1

void    move_player(t_game *game);
void    rotate_player(t_game *game);

void	init_player(t_player *player, double x, double y, double angle)
{
	if (x != UNDEFINED)
		player->vec_pos.x = x;
	if (y != UNDEFINED)
		player->vec_pos.y = y;
	if (angle != UNDEFINED)
		player->camera_angle = angle;
}
