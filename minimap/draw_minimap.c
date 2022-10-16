#include "cub3d.h"
#include "mlx.h"

void	put_one_square_pixels_to_screen(t_game *game, int x, int y, \
														unsigned int color)
{
	const int			init_x = x;
	const int			end_x = x + game->minimap.pixel_per_square;
	const int			end_y = y + game->minimap.pixel_per_square;
	const t_info		*info = &game->info;

	while (y < end_y)
	{
		x = init_x;
		while (x < end_x)
		{
			mlx_pixel_put(info->mlx_ptr, info->win_ptr, x, y, color);
			x++;
		}
		y++;
	}
}

void	draw_camera_angle(t_player *player, t_img_data *view_data, int pps)
{
	const int		player_x = (int)(player->vec_pos.x * pps);
	const int		player_y = (int)(player->vec_pos.y * pps);
	const double	angle = player->camera_angle;
	int				pos[2];

	set_range(pos, player_x + pps / 2 - !(pps % 2), player_y + pps / 2 - !(pps % 2));
	pos[X] += (int)(pps * cut_point(cos(deg2rad(angle)), 6));
	pos[Y] += (int)(pps * cut_point(sin(deg2rad(angle)), 6));
	put_one_square_pixels_to_img(view_data, pos, pps / 4, create_trgb(0,255,0,0));
}

void	draw_player(t_player *player, t_img_data *view_data, int pps)
{
	const int	player_x = (int)(player->vec_pos.x * pps);
	const int	player_y = (int)(player->vec_pos.y * pps);
	int			pos[2];

	set_range(pos, player_x, player_y);
	put_one_square_pixels_to_img(view_data, pos, pps, create_trgb(0,0,0,255));
}

void	draw_minimap(t_game *game)
{
	const int			pps = game->minimap.pixel_per_square;
	const t_img_data	*minimap_data = &game->minimap.map_img_data;
	t_img_data			*view_data;
	int					x;
	int					y;

	view_data = &game->view_data;
	y = 0;
	while (y < game->minimap.height)
	{
		x = 0;
		while (x < game->minimap.width)
		{
			*(unsigned int *)(view_data->addr + (y * view_data->line_length + \
				x * (view_data->bits_per_pixel / 8))) = *(unsigned int *)(minimap_data->addr + \
				(y * minimap_data->line_length + x * (minimap_data->bits_per_pixel / 8)));
			x++;
		}
		y++;
	}
	draw_player(&game->player, &game->view_data, pps);
	draw_camera_angle(&game->player, &game->view_data, pps);
}