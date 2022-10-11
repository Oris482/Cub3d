#include "../mlx/mlx.h"
#include "../cub3d.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>

void    make_ceiling_floor_image(t_game *game);
void	draw_ceiling_floor(t_game *game, int x, int y_top, int y_bottom, double fog_value);

void	check_argv(int argc, char *argv[], t_game *game);

void	graphic_resource_init(t_info *info)
{
	info->mlx_ptr = mlx_init();
	mlx_get_screen_size(info->mlx_ptr, &info->screen_x, &info->screen_y);
	info->screen_x *= 0.6;
	info->screen_y *= 0.7;
	info->fov_h = 60.0;
	info->fov_v = (double)info->screen_y / (double)info->screen_x \
															* info->fov_h;
	printf("screen_witdh : %d\nscreen_height : %d\n", info->screen_x, info->screen_y);
	printf("fov_h : %f\nfov_v : %f\n", info->fov_h, info->fov_v);
	info->win_ptr = mlx_new_window(info->mlx_ptr, info->screen_x, \
												info->screen_y, "cub3D");
}

int	main_loop(t_game *game)
{
	// printf("X : %f Y: %f Angle: %f\n", game->player.vec_pos.x, game->player.vec_pos.y, game->player.camera_angle);
	// 여기서는 눌린 키가 있어야 그리기 때문에 스프라이트는 없다고 가정함.
	if (game->pressed_keyset != 0)
	{
		// if (game->pressed_keyset | KEY_WASD)
		// 	move_player(game);
		// if (game->pressed_keyset | KEY_ARROW)
		// 	rotate_player(game);
		;
	}
	return (0);
}

int	exit_game(int exit_status)
{
	exit(exit_status);
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
	if (key == KEY_LSHIFT)
		*keyset_ptr |= KEYSET_LSHIFT;
	return (0);
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
	if (key == KEY_LSHIFT)
		*keyset_ptr &= ~KEYSET_LSHIFT;
	return (0);
}

void	loop(t_game *game)
{
	mlx_loop_hook(game->info.mlx_ptr, main_loop, game);
	mlx_hook(game->info.win_ptr, KEY_PRESS_EVENT, 1L << 0, ft_key_press, game);
	mlx_hook(game->info.win_ptr, KEY_RELEASE_EVENT, 1L << 0, ft_key_release, game);
	mlx_hook(game->info.win_ptr, KEY_EXIT, 0, exit_game, 0);
	mlx_loop(game->info.mlx_ptr);
}

void	init_game(int argc, char **argv, t_game *game)
{
	static char* const	tmp_map[] = {
		[0] = "1111111111",
		[1] = "1001110001",
		[2] = "1000000001",
		[3] = "1000000001",
		[4] = "1000000001",
		[5] = "10000N0001",
		[6] = "1111111111"
	};
	game->player.move_speed = 0.001;
	game->player.rotate_speed = 0.01;
	game->player.camera_angle = 0;
	game->pressed_keyset = 0;
}

int	main(int argc, char *argv[])
{
	t_game	game;
	int		y_top;
	int		y_bottom;
	int		x;

	init_game(argc, argv, &game);
	graphic_resource_init(&game.info);
	make_ceiling_floor_image(&game);
	check_argv(argc, argv, &game);
	y_top = 200;
	y_bottom = 300;
	x = 0;
	while (x <= game.info.screen_x / 2)
	{
		draw_ceiling_floor(&game, x, y_top - (x / 6), y_bottom - (x / 6), 0.2);
		x++;
	}
	while (x <= game.info.screen_x)
	{
		draw_ceiling_floor(&game, x, (y_top - (game.info.screen_x / 12)) + ((x - (game.info.screen_x / 2)) / 6), (y_bottom - (game.info.screen_x / 12)) + ((x - (game.info.screen_x / 2)) / 6), 0.2);
		x++;
	}
	mlx_put_image_to_window(game.info.mlx_ptr, game.info.win_ptr, game.bg_data.img, 0, 0);
	loop(&game);
	return (0);
}
