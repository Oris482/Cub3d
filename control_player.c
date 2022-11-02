/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_player.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaemjeon <jaemjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 16:00:36 by jaemjeon          #+#    #+#             */
/*   Updated: 2022/11/02 16:00:38 by jaemjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "mlx.h"

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

void	boost_key_off(t_game *game)
{
	game->player.rotate_speed_v /= 2;
	game->player.rotate_speed_h /= 2;
	game->player.move_speed /= 2;
	game->player.view_trans = 0;
}

void	boost_key_on(t_game *game)
{
	game->player.rotate_speed_v *= 2;
	game->player.rotate_speed_h *= 2;
	game->player.move_speed *= 2;
	game->player.view_trans = 180;
}

int	mouse_switch(int key_code, t_game *game)
{
	t_info *const	info = &game->info;

	if (key_code == KEY_M)
	{
		if (game->info.using_mouse == FALSE)
		{
			mlx_mouse_move(info->win_ptr, \
									info->screen_x / 2, info->screen_y / 2);
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
