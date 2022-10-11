#include "../mlx/mlx.h"
#include "../cub3d.h"
#include <stdio.h>

#define START	0
#define END		1

int create_trgb(unsigned char t, unsigned char r, \
								unsigned char g, unsigned char b)
{
	return (*(int *)(unsigned char [4]){b, g, r, t});
}

unsigned char	get_r(int trgb)
{
	return (((unsigned char *)&trgb)[2]);
}

unsigned char	get_g(int trgb)
{
	return (((unsigned char *)&trgb)[1]);
}

unsigned char	get_b(int trgb)
{
	return (((unsigned char *)&trgb)[0]);
}

void	pixel_put_to_img(t_bg_data *bg_data, int x, \
							int range_y[2], int color, double base_transperency)
{
	char	*dst;
	int		y;
	int		step;
	int		sign;
	double		gradiant;

	step = range_y[END] - range_y[START];
	sign = 1;
	if (step < 0)
	{
		sign = -1;
		step *= -1;
	}
	y = range_y[START];
	gradiant = base_transperency;
	while (y != range_y[END])
	{
		gradiant += (1.0 - base_transperency) / step;
		dst = bg_data->addr + (y * bg_data->line_length + x \
									* (bg_data->bits_per_pixel / 8));
		*(unsigned int*)dst = create_trgb(0, get_r(color) * gradiant, \
							get_g(color) * gradiant, get_b(color) * gradiant);
		y += sign;
	}
}

void	set_range_x(int x[2], int x_start, int x_end)
{
	x[START] = x_start;
	x[END] = x_end;
}

void	set_range_y(int y[2], int y_start, int y_end)
{
	y[START] = y_start;
	y[END] = y_end;
}

void    make_ceiling_floor_image(t_game *game)
{
	int			x;
	int			y[2];

    game->bg_data.img = mlx_new_image(game->info.mlx_ptr, game->info.screen_x, \
														game->info.screen_y);
    game->bg_data.addr = mlx_get_data_addr(game->bg_data.img, &game->bg_data.bits_per_pixel, \
										&game->bg_data.line_length, &game->bg_data.endian);
}

void	draw_ceiling_floor(t_game *game, int x, int y_top, int y_bottom, double fog_value)
{
	const int	screen_y = game->info.screen_y;
	int			y[2];

	if (y_top > 0)
	{
		set_range_y(y, y_top, 0);
		pixel_put_to_img(&game->bg_data, x, y, game->ceiling_color, fog_value);
	}
	if (y_bottom < screen_y)
	{
		set_range_y(y, y_bottom, screen_y);
		pixel_put_to_img(&game->bg_data, x, y, game->floor_color, fog_value);
	}
}