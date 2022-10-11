#include "../mlx/mlx.h"
#include "../cub3d.h"
#include <stdio.h>

#define START	0
#define END		1

static void	_pixel_put_to_img(t_bg_data *bg_data, int x, int range_y[2], \
										int color, double base_transperency)
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

static void	_set_range_x(int x[2], int x_start, int x_end)
{
	x[START] = x_start;
	x[END] = x_end;
}

static void	_set_range_y(int y[2], int y_start, int y_end)
{
	y[START] = y_start;
	y[END] = y_end;
}

void	draw_ceiling_floor(t_game *game, int x, int y_top, int y_bottom, double fog_value)
{
	const int	screen_y = game->info.screen_y;
	int			y[2];

	if (y_top > 0)
	{
		_set_range_y(y, y_top, 0);
		_pixel_put_to_img(&game->bg_data, x, y, game->ceiling_color, fog_value);
	}
	if (y_bottom < screen_y)
	{
		_set_range_y(y, y_bottom, screen_y);
		_pixel_put_to_img(&game->bg_data, x, y, game->floor_color, fog_value);
	}
}
