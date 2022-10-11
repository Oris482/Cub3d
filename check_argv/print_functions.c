/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaesjeon <jaesjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 19:05:36 by jaesjeon          #+#    #+#             */
/*   Updated: 2022/10/11 19:12:54 by jaesjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ft_string.h"

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
					name, texture.filename, texture.img, \
					texture.texture_width, texture.texture_height);
}

static void	_print_rgb_info(char *name, unsigned int color)
{
	printf("%s\t:\t[R %d].[G %d].[B %d]\n", name, get_r(color), \
										get_g(color), get_b(color));
}

void	print_game_info(t_game *game)
{
	const t_info	*info = &game->info;
	const t_texture	*texture = game->texture;

	printf("************************ cub3d start ************************\n");
	printf("-------------------------------------------------------------\n");
	printf("--------------------------VIEW INFO--------------------------\n");
	printf("-------------------------------------------------------------\n");
	printf("@ Window size\t:\t%d X %d\n", info->screen_x, info->screen_y);
	printf("@ FOV_H\t\t:\t%f\n", info->fov_h);
	printf("@ FOV_V\t\t:\t%f\n", info->fov_v);
	printf("-------------------------------------------------------------\n");
	printf("-------------------------WALL TEXTURE------------------------\n");
	printf("-------------------------------------------------------------\n");
	_print_texture_info("@ EAST", texture[EA]);
	_print_texture_info("@ WEST", texture[WE]);
	_print_texture_info("@ SOUTH", texture[SO]);
	_print_texture_info("@ NORTH", texture[NO]);
	printf("-------------------------------------------------------------\n");
	printf("----------------------FLOOR AND CEILING----------------------\n");
	printf("-------------------------------------------------------------\n");
	_print_rgb_info("@ FLOOR ", game->floor_color);
	_print_rgb_info("@ CEILING", game->ceiling_color);
	printf("*************************************************************\n");
}
