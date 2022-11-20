/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaesjeon <jaesjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 19:05:36 by jaesjeon          #+#    #+#             */
/*   Updated: 2022/11/12 19:11:42 by jaesjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h>
#include <unistd.h>

void	exit_with_err(char *custom_msg, int exit_code)
{
	printf("ERROR\n:");
	if (custom_msg != NULL)
		printf("%s\n", custom_msg);
	else
		perror(NULL);
	exit(exit_code);
}

static void	_print_texture_info(char *name, t_texture texture)
{
	printf("%s\n -filename\t:\t%s\n -address\t:\t%p\n -size\t\t:\t%d X %d\n", \
					name, texture.filename, texture.img_data.img, \
					texture.texture_width, texture.texture_height);
}

static void	_print_rgb_info(char *name, unsigned int color)
{
	printf("%s\t:\t[R %d].[G %d].[B %d]\n", name, get_r(color), \
										get_g(color), get_b(color));
}

void	print_map(char **map, int end_x, int end_y)
{
	int	x;
	int	y;

	y = 0;
	while (y < end_y)
	{
		x = 0;
		while (x < end_x)
		{
			write(1, &map[y][x], 1);
			x++;
		}
		write(1, "\n", 1);
		y++;
	}
}

void	print_game_info(t_game *game)
{
	const t_info	*info = &game->info;
	const t_player	*player = &game->player;
	const t_texture	*texture = game->texture;

	printf("----------------------------------------------------------\n");
	printf("------------------------VIEW INFO-------------------------\n\n");
	printf("@ Window size\t:\t%d X %d\n", info->screen_x, info->screen_y);
	printf("@ FOV_H\t\t:\t%f\n", info->fov_h);
	printf("@ FOV_V\t\t:\t%f\n", info->fov_v);
	printf("@ Player info\t:\tPosition[%d, %d], Camera angle[%.1f]\n", \
		(int)player->vec_pos.x, (int)player->vec_pos.y, player->camera_angle_h);
	printf("@ Map info\t:\t%d X %d\n", info->map_x, info->map_y);
	print_map(info->map, info->map_x, info->map_y);
	printf("\n-----------------------WALL TEXTURE-----------------------\n\n");
	_print_texture_info("@ EAST", texture[EA]);
	_print_texture_info("@ WEST", texture[WE]);
	_print_texture_info("@ SOUTH", texture[SO]);
	_print_texture_info("@ NORTH", texture[NO]);
	printf("\n--------------------FLOOR AND CEILING---------------------\n\n");
	_print_rgb_info("@ FLOOR ", game->floor_color);
	_print_rgb_info("@ CEILING", game->ceiling_color);
	printf("\n----------------------------------------------------------\n");
}
