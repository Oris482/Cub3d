#include "./mlx/mlx.h"
#include "./mlx/mlx_opengl.h"
#include "cub3d.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>

void	graphic_resource_init(t_info *info)
{
	info->mlx_ptr = mlx_init();
	info->win_ptr = \
				mlx_new_window(info->mlx_ptr, SIZE_WIN_X, SIZE_WIN_Y, "cub3D");
}

void	draw(t_game *game)
{

}

t_vector2	get_move_angle(t_game *game, const unsigned int key)
{
	// t_vector2	vec_delta_move;
	// double		x
	// if ((pressed_key | KEY_W) != 0 && (pressed_key | KEY_S) == 0)
	// {

	// }
	// else if ((pressed_key | KEY_W) 0= 0 && (pressed_key | KEY_S) != 0)
	// {
	// 	move_delta_x += game->player.move_speed * cos(game->player.camera_angle);
	// 	move_delta_y += game->player.move_speed * sin(game->player.camera_angle);
	// }
	// if ((pressed_key | KEY_W) != 0 && (pressed_key | KEY_S) == 0)
	// {
	// 	move_delta_x += game->player.move_speed * cos(game->player.camera_angle);
	// 	move_delta_y += game->player.move_speed * sin(game->player.camera_angle);
	// }
	// else if ((pressed_key | KEY_W) 0= 0 && (pressed_key | KEY_S) != 0)
	// {
	// 	move_delta_x += game->player.move_speed * cos(game->player.camera_angle);
	// 	move_delta_y += game->player.move_speed * sin(game->player.camera_angle);
	// }
}

void	move(t_game *game)
{
	const unsigned int	pressed_key = game->pressed_keyset;
	double				mcamera_angle;
	t_vector2			move_angle;
	double				move_delta_x;
	double				move_delta_y;

	mcamera_angle = 0;
}

void	turn_view(t_game *game)
{
	// mlx_hook(game->info.win_ptr, 2, 1L << 0, )
}

int	main_loop(t_game *game)
{
	printf("%d\n", game->pressed_keyset);
	// 여기서는 눌린 키가 있어야 그리기 때문에 스프라이트는 없다고 가정함.
	if (game->pressed_keyset != 0)
	{
		if (game->pressed_keyset | KEY_WASD)
			move(game);
		if (game->pressed_keyset | KEY_ARROW)
			turn_view(game);
		draw(game);
	}
	return (0);
}

int	exit_game(int exit_status)
{
	exit(0);
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
	// if (argc != 2)
	// {
	// 	exit(22);
	// 	perror(strerror(22));
	// }
	game->player.move_speed = 1;
	game->player.rotate_speed = 1;
	game->player.camera_angle = 0;
	game->pressed_keyset = 0;
	// 여기서 파일오픈해서 파싱해서 넣어야함.
	game->map = (char **)tmp_map;
}

int	main(int argc, char *argv[])
{
	t_game	game;

	init_game(argc, argv, &game);
	graphic_resource_init(&game.info);
	loop(&game);
	return (0);
}
