#include "cub3d.h"
#include "mlx.h"

void	put_one_square_pixels_to_img(t_img_data *img_data, int pos[2], \
												int pps, unsigned int color)
{
	char				*dst;
	const int			init_x = pos[X];
	const int			end_x = pos[X] + pps;
	const int			end_y = pos[Y] + pps;

	while (pos[Y] < end_y)
	{
		pos[X] = init_x;
		while (pos[X] < end_x)
		{
			dst = img_data->addr + (pos[Y] * img_data->line_length + \
									pos[X] * (img_data->bits_per_pixel / 8));
			*(unsigned int *)dst = color;
			pos[X] += 1;
		}
		pos[Y] += 1;
	}
}

void	put_pixels_to_minimap_img(t_info *info, t_minimap *minimap)
{
	int			idx_x;
	int			idx_y;
	int			pos[2];
	const int	pps = minimap->pixel_per_square;

	idx_y = 0;
	while (idx_y < info->map_y)
	{
		idx_x = 0;
		while (idx_x < info->map_x)
		{
			set_range(pos, idx_x * pps, idx_y * pps);
			if (info->map[idx_y][idx_x] == WALL)
				put_one_square_pixels_to_img(&minimap->map_img_data, pos, \
												pps, create_trgb(0, 0, 0, 0));
			else if (info->map[idx_y][idx_x] == NONE)
				put_one_square_pixels_to_img(&minimap->map_img_data, pos, \
												pps, create_trgb(170, 0, 0, 0));
			else
				put_one_square_pixels_to_img(&minimap->map_img_data, pos, \
											pps, create_trgb(0, 255, 255, 255));
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
	
	printf("Calculate minimap size based on screen size\n");
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
	printf("Minimap size\t\t:\t%d X %d\n", minimap->width, minimap->height);
	printf("pixels per one square\t:\t%d\n", minimap->pixel_per_square);
}

void	make_minimap_image(t_game *game)
{
	t_minimap	*minimap;
	t_img_data	*map_img_data;
	t_img_data	*player_img_data;

	minimap = &game->minimap;
	map_img_data = &minimap->map_img_data;
	calculate_minimap_size(&game->info, minimap);
	map_img_data->img = mlx_new_image(game->info.mlx_ptr, \
											minimap->width, minimap->height);
	if (map_img_data->img == NULL)
		exit_with_err("mlx function error", E_PERM);
	map_img_data->addr = mlx_get_data_addr(map_img_data->img, \
		&map_img_data->bits_per_pixel, &map_img_data->line_length, \
														&map_img_data->endian);
	put_pixels_to_minimap_img(&game->info, minimap);
}
