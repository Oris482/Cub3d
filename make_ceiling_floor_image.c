#include "../mlx/mlx.h"
#include "../cub3d.h"
#include <stdio.h>

typedef struct outside_wall_info
{
	int	x;
	int	range_y[2];
}	t_outside_wall_info;

static void	_pixel_put_to_img(t_img_data *view_img, int color, \
					t_outside_wall_info outside_wall, double base_transperency)
{
	char	*dst;
	int		step;
	int		sign;
	int		y;
	double	gradiant;

	step = outside_wall.range_y[END] - outside_wall.range_y[START];
	sign = 1;
	if (step < 0)
	{
		sign = -1;
		step *= -1;
	}
	gradiant = base_transperency;
	y = outside_wall.range_y[START];
	while (y != outside_wall.range_y[END])
	{
		gradiant += (1.0 - base_transperency) / step;
		dst = view_img->addr + (y * view_img->line_length + \
								outside_wall.x * (view_img->bits_per_pixel / 8));
		*(unsigned int *)dst = create_trgb(0, get_r(color) * gradiant, \
							get_g(color) * gradiant, get_b(color) * gradiant);
		y += sign;
	}
}

void	__set_range(int arg[2], int arg_start, int arg_end)
{
	arg[START] = arg_start;
	arg[END] = arg_end;
}

void	draw_ceiling_floor(t_game *game, int x, int wall_y[2], double fog_value)
{
	const int			screen_y = game->info.screen_y;
	t_outside_wall_info	outside_wall;

	outside_wall.x = x;
	if (wall_y[START] > 0)
	{
		__set_range(outside_wall.range_y, wall_y[START], 0);
		_pixel_put_to_img(&game->view_data, game->ceiling_color, \
										outside_wall, fog_value);
	}
	if (wall_y[END] < screen_y)
	{
		__set_range(outside_wall.range_y, wall_y[END], screen_y);
		_pixel_put_to_img(&game->view_data, game->floor_color, \
										outside_wall, fog_value);
	}
}
