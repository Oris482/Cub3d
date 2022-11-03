/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaemjeon <jaemjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 16:00:12 by jaemjeon          #+#    #+#             */
/*   Updated: 2022/11/02 16:00:38 by jaemjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "cub3d.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>

int	main_loop(t_game *game)
{
	if (game->pressed_keyset != 0)
	{
		if (game->pressed_keyset & KEY_WASD)
			move_player(&game->player, game->info.map, game->pressed_keyset);
		if (game->pressed_keyset & KEY_ARROW)
			rotate_player_key(game, game->pressed_keyset);
	}
	if (game->info.using_mouse == TRUE)
		rotate_player_mouse(game);
	draw_screen(game);
	draw_minimap(game);
	mlx_sync(MLX_SYNC_IMAGE_WRITABLE, game->view_data.img);
	mlx_put_image_to_window(game->info.mlx_ptr, game->info.win_ptr, \
								game->view_data.img, 0, 0);
	mlx_sync(MLX_SYNC_WIN_CMD_COMPLETED, game->info.win_ptr);
	return (0);
}

void	loop(t_game *game)
{
	mlx_loop_hook(game->info.mlx_ptr, main_loop, game);
	mlx_hook(game->info.win_ptr, KEY_PRESS_EVENT, 1L << 0, ft_key_press, game);
	mlx_hook(game->info.win_ptr, \
							KEY_RELEASE_EVENT, 1L << 1, ft_key_release, game);
	mlx_hook(game->info.win_ptr, KEY_EXIT, 0, exit_game, 0);
	mlx_loop(game->info.mlx_ptr);
}

int	main(int argc, char *argv[])
{
	t_game	game;

	graphic_resource_init(&game);
	player_handle_setting(&game);
	check_argv(argc, argv, &game);
	print_game_info(&game);
	make_minimap_image(&game);
	loop(&game);
	return (0);
}
