/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaemjeon <jaemjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:59:46 by jaemjeon          #+#    #+#             */
/*   Updated: 2022/11/02 15:59:47 by jaemjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "mlx.h"

void	malloc_resources(t_game *game)
{
	game->ray_data = (t_ray *)malloc(sizeof(t_ray) * game->info.screen_x);
	game->wall_pixel = \
			(t_vector2_d *)malloc(sizeof(t_vector2_d) * game->info.screen_x);
	game->wall_line = \
			(t_vector2 *)malloc(sizeof(t_vector2) * game->info.screen_x);
	if (!game->ray_data || !game->wall_pixel || !game->wall_line)
		exit_with_err("Malloc error", E_NOMEM);
}

void	graphic_resource_init(t_game *game)
{
	t_info *const	info = &game->info;
	int				idx;

	ft_memset(game, 0, sizeof(t_game));
	info->mlx_ptr = mlx_init();
	if (info->mlx_ptr == NULL)
		exit_with_err("mlx function error", E_PERM);
	mlx_get_screen_size(info->mlx_ptr, &info->screen_x, &info->screen_y);
	info->screen_x *= 0.6;
	info->screen_y *= 0.6;
	info->fov_h = 60.0;
	info->fov_v = (double)info->screen_y / (double)info->screen_x \
															* info->fov_h;
	info->win_ptr = mlx_new_window(info->mlx_ptr, info->screen_x, \
												info->screen_y, "cub3D");
	game->view_data.img = mlx_new_image(game->info.mlx_ptr, \
		game->info.screen_x, game->info.screen_y);
	if (!info->win_ptr || !game->view_data.img)
		exit_with_err("mlx function error", E_PERM);
	game->view_data.addr = mlx_get_data_addr(game->view_data.img, \
		&game->view_data.bits_per_pixel, &game->view_data.line_length, \
		&game->view_data.endian);
	malloc_resources(game);
}

void	player_handle_setting(t_game *game)
{
	game->player.move_speed = 0.2;
	game->player.rotate_speed_h = 3.0;
	game->player.rotate_speed_v = 30;
	game->player.vertical_dist_pixel = 0;
	game->player.view_trans = 0;
	game->info.using_mouse = TRUE;
	mlx_mouse_hide();
	mlx_mouse_move(game->info.win_ptr, \
							game->info.screen_x / 2, game->info.screen_y / 2);
}
