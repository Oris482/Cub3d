#include "../cub3d.h"

void	init_ray(t_game *game)
{
	int				idx_x;
	const double	delta_angle = game->info.fov_h / game->info.screen_x;

	idx_x = 0;
	game->ray_data = (t_ray *)malloc(sizeof(t_ray) * game->info.screen_x);
	while (idx_x < game->info.screen_x)
	{
		game->ray_data[idx_x].cast_angle = \
								delta_angle * idx_x - (game->info.fov_h / 2);
		idx_x++;
	}
}

void	cast_sigle_ray(t_ray *ray, t_info *info)
{
	
}

void	ray_cast(t_game *game)
{
	int	idx_x;

	idx_x = 0;
	while (idx_x < game->info.screen_x)
	{
		cast_sigle_ray(game->ray_data, game->info);
		idx_x++;
	}
}

void	draw_wall(t_game *game)
{
	ray_cast(game);
}