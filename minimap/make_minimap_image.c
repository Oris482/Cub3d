#include "cub3d.h"
#include "mlx.h"

void	put_one_square_pixels(t_minimap *minimap, int x, int y, \
														unsigned int color)
{
	const t_img_data	*img_data = &minimap->img_data;
	char				*dst;
	const int			init_x = x;
	const int			end_x = x + minimap->pixel_per_square;
	const int			end_y = y + minimap->pixel_per_square;

	while (y < end_y)
	{
		x = init_x;
		while (x < end_x)
		{
			dst = img_data->addr + (y * img_data->line_length + \
									x * (img_data->bits_per_pixel / 8));
			*(unsigned int *)dst = color;
			x++;
		}
		y++;
	}
}

void	draw_minimap(t_info *info, t_minimap *minimap)
{
	int			idx_x;
	int			idx_y;
	const int	pps = minimap->pixel_per_square;

	idx_y = 0;
	while (idx_y < info->map_y)
	{
		idx_x = 0;
		while (idx_x < info->map_x)
		{
			if (info->map[idx_y][idx_x] == WALL)
				put_one_square_pixels(minimap, idx_x * pps, idx_y * pps, create_trgb(0, 0, 0, 0));
			else if (info->map[idx_y][idx_x] == NONE)
				put_one_square_pixels(minimap, idx_x * pps, idx_y * pps, create_trgb(170, 0, 0, 0));
			else
				put_one_square_pixels(minimap, idx_x * pps, idx_y * pps, create_trgb(0, 255, 255, 255));
			idx_x++;
		}
		idx_y++;
	}
}

void	calculate_minimap_size(t_info *info, t_minimap *minimap)
{
	double			minimap_size[2];
	const int		screen_x = info->screen_x;
	const int		screen_y = info->screen_y;
	
	minimap_size[X] = info->map_x;
	minimap_size[Y] = info->map_y;
	while (minimap_size[X] < (screen_x * 0.3) && \
									minimap_size[Y] < (screen_y * 0.3))
	{
		minimap_size[X] *= 1.01;
		minimap_size[Y] *= 1.01;
	}
	minimap->width = info->map_x * (int)(minimap_size[X] / info->map_x);
	minimap->height = info->map_y * (int)(minimap_size[Y] / info->map_y);
	minimap->pixel_per_square = minimap_size[X] / info->map_x;
	printf("x : %d, y : %d, ratio : %f\n", minimap->width, minimap->height, \
						(double)(minimap->height) / (double)(minimap->width));
	printf("pixels per one square : %d\n", minimap->pixel_per_square);
}

void	make_minimap_image(t_game *game)
{
	t_minimap	*minimap;
	t_img_data	*img_data;

	minimap = &game->minimap;
	img_data = &minimap->img_data;
	calculate_minimap_size(&game->info, minimap);
	img_data->img = mlx_new_image(game->info.mlx_ptr, \
											minimap->width, minimap->height);
	if (img_data->img == NULL)
		exit_with_err("mlx function error", E_PERM);
	img_data->addr = mlx_get_data_addr(img_data->img, \
		&img_data->bits_per_pixel, &img_data->line_length, &img_data->endian);
	draw_minimap(&game->info, minimap);
	mlx_put_image_to_window(game->info.mlx_ptr, game->info.win_ptr, minimap->img_data.img, 0, 0);
}
