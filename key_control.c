#include "cub3d.h"
#include "mlx.h"

void	set_keyset(unsigned int * const keyset_ptr, int key)
{
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
}

int	ft_key_press(int key, t_game *game)
{
	unsigned int * const	keyset_ptr = &game->pressed_keyset;

	if (key == KEY_ESC)
	{
		mlx_destroy_window(game->info.mlx_ptr, game->info.win_ptr);
		exit_game(0);
	}
	set_keyset(keyset_ptr, key);
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
