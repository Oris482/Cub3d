#include "mlx.h"
#include "cub3d.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>

void	graphic_resource_init(t_game *game)
{
	t_info * const	info = &game->info;
	int		idx;

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
	game->ray_data = (t_ray *)malloc(sizeof(t_ray) * info->screen_x);
	game->wall_pixel = (t_vector2_d *)malloc(sizeof(t_vector2_d) * game->info.screen_x);
	game->wall_line = (t_vector2 *)malloc(sizeof(t_vector2) * game->info.screen_x);
	if (!game->ray_data || !game->wall_pixel || !game->wall_line)
		exit_with_err("Malloc error", E_NOMEM);
}

int	mouse_switch(int key_code, t_game *game)
{
	t_info * const	ifo = &game->info;
	if (key_code == KEY_M)
	{
		if (game->info.using_mouse == FALSE)
		{
			mlx_mouse_move(ifo->win_ptr, ifo->screen_x / 2, ifo->screen_y / 2);
			game->info.using_mouse = TRUE;
			mlx_mouse_hide();
		}
		else
		{
			game->info.using_mouse = FALSE;
			mlx_mouse_show();
		}
	}
	return (TRUE);
}

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
	mlx_put_image_to_window(game->info.mlx_ptr, game->info.win_ptr,
							game->view_data.img, 0, 0);
	mlx_sync(MLX_SYNC_WIN_CMD_COMPLETED, game->info.win_ptr);
	return (0);
}

int	exit_game(int exit_status)
{
	exit(exit_status);
}

void	turn_cardinal_points(int key, t_player *player)
{
	if (key == KEY_H)
		player->camera_angle_h = 180.0;
	else if (key == KEY_J)
		player->camera_angle_h = 90.0;
	else if (key == KEY_K)
		player->camera_angle_h = 270.0;
	else if (key == KEY_L)
		player->camera_angle_h = 0.0;
	else
		return ;
}

void	boost_key_on(t_game *game)
{
	game->player.rotate_speed_v *= 2;
	game->player.rotate_speed_h *= 2;
	game->player.move_speed *= 2;
	game->player.view_trans = 180;
}

int	ft_key_press(int key, t_game *game)
{
	unsigned int * const	keyset_ptr = &game->pressed_keyset;

	if (key == KEY_ESC)
	{
		mlx_destroy_window(game->info.mlx_ptr, game->info.win_ptr);
		exit_game(0);
	}
	if (key == KEY_A)
		*keyset_ptr |= KEYSET_A;
	if (key == KEY_S)
		*keyset_ptr |= KEYSET_S;
	if (key == KEY_D)
		*keyset_ptr |= KEYSET_D;
	if (key == KEY_W)
		*keyset_ptr |= KEYSET_W;
	if (key == KEY_LA)
		*keyset_ptr |= KEYSET_LA;
	if (key == KEY_RA)
		*keyset_ptr |= KEYSET_RA;
	if (key == KEY_DA)
		*keyset_ptr |= KEYSET_DA;
	if (key == KEY_UA)
		*keyset_ptr |= KEYSET_UA;
	if (key == KEY_H || key == KEY_J || key == KEY_K || key == KEY_L)
		turn_cardinal_points(key, &game->player);
	if (key == KEY_LSHIFT)
	{
		*keyset_ptr |= KEYSET_LSHIFT;
		boost_key_on(game);
	}
	if (key == KEY_M)
		mouse_switch(KEY_M, game);
	return (0);
}

void	boost_key_off(t_game *game)
{
	game->player.rotate_speed_v /= 2;
	game->player.rotate_speed_h /= 2;
	game->player.move_speed /= 2;
	game->player.view_trans = 0;
}

int	ft_key_release(int key, t_game *game)
{
	unsigned int * const	keyset_ptr = &game->pressed_keyset;

	if (key == KEY_A)
		*keyset_ptr &= ~KEYSET_A;
	if (key == KEY_S)
		*keyset_ptr &= ~KEYSET_S;
	if (key == KEY_D)
		*keyset_ptr &= ~KEYSET_D;
	if (key == KEY_W)
		*keyset_ptr &= ~KEYSET_W;
	if (key == KEY_LA)
		*keyset_ptr &= ~KEYSET_LA;
	if (key == KEY_RA)
		*keyset_ptr &= ~KEYSET_RA;
	if (key == KEY_DA)
		*keyset_ptr &= ~KEYSET_DA;
	if (key == KEY_UA)
		*keyset_ptr &= ~KEYSET_UA;
	if (key == KEY_LSHIFT)
	{
		*keyset_ptr &= ~KEYSET_LSHIFT;
		boost_key_off(game);
	}
	return (0);
}

void	loop(t_game *game)
{
	mlx_loop_hook(game->info.mlx_ptr, main_loop, game);
	mlx_hook(game->info.win_ptr, KEY_PRESS_EVENT, 1L << 0, ft_key_press, game);
	mlx_hook(game->info.win_ptr, KEY_RELEASE_EVENT, 1L << 1, ft_key_release, game);
	mlx_hook(game->info.win_ptr, KEY_EXIT, 0, exit_game, 0);
	mlx_loop(game->info.mlx_ptr);
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
