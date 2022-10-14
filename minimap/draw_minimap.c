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

void	draw_player(t_game *game, int pps)
{
	const t_player	*player = &game->player;
	const int		player_x = (int)(player->vec_pos.x * pps);
	const int		player_y = (int)(player->vec_pos.y * pps);

	mlx_put_image_to_window(game->info.mlx_ptr, game->info.win_ptr, \
						game->minimap.player_img_data.img, player_x, player_y);
}

void	draw_minimap(t_game *game)
{
	const int	pps = game->minimap.pixel_per_square;

	mlx_put_image_to_window(game->info.mlx_ptr, game->info.win_ptr, game->minimap.map_img_data.img, 0, 0);
	draw_player(game, pps);
}