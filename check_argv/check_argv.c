#include "../cub3d.h"
#include "../mlx/mlx.h"
#include <fcntl.h>
#include "ft_string.h"

enum	e_rgb
{
	R = 0,
	G,
	B
};

void	set_wall_texture(t_game *game, int element_identifier, \
												char *texture_file)
{
	t_texture	*target;

	target = &game->texture[element_identifier];
	if (target->filename)
		free(target->filename);
	target->filename = ft_strcpy(texture_file);
	if (target->img)
		mlx_destroy_image(game->info.mlx_ptr, target->img);
	target->img = mlx_xpm_file_to_image(game->info.mlx_ptr, texture_file, \
								&target->texture_width, &target->texture_height);
	if (target->img == NULL)
		exit_with_err("can't open texture file", E_PERM);
}

void	set_floor_ceiling_color(t_game *game, int element_identifier, \
															char *rgb_str)
{
	unsigned int	*target;
	unsigned char	color_value[3];
	int				idx;

	if (element_identifier == F)
		target = &game->floor_color;
	else
		target = &game->ceiling_color;
	idx = R;
	while (idx <= B)
	{
		while (*rgb_str && (*rgb_str == ' ' || *rgb_str == '\t'))
			rgb_str++;
		if (*rgb_str == '\0')
			exit_with_err("floor, ceiling color value error", E_PERM);
		color_value[idx] = get_color_value(&rgb_str);
		idx++;
		if (*rgb_str == ',')
			rgb_str++;
	}
	*target = create_trgb(0, color_value[R], color_value[G], color_value[B]);
}

void	get_elements_info(int fd, unsigned char *elements_flag, t_game *game)
{
	char	*line;
	char	*pair[2];
	int		element_identifier;

	while (*elements_flag != 0b00111111)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		seperate_by_key_value(pair, line);
		if (pair[KEY] == NULL)
		{
			free(line);
			continue ;
		}
		element_identifier = get_element_identifier(pair[KEY]);
		if (element_identifier == ERROR || pair[VALUE] == NULL)
			exit_with_err("invalid element source", E_PERM);
		else if (element_identifier < F)
			set_wall_texture(game, element_identifier, pair[VALUE]);
		else
			set_floor_ceiling_color(game, element_identifier, pair[VALUE]);
		*elements_flag |= get_elements_flag_bit(element_identifier);
		multi_free(pair[KEY], pair[VALUE], line, NULL);
	}
}

void	check_cub_elements(int fd, t_game *game)
{
	unsigned char	elements_flag;

	elements_flag = 0;
	get_elements_info(fd, &elements_flag, game);
	if (elements_flag != 0b00111111)
		exit_with_err("some elements source missing", E_PERM);
}

void	check_argv(int argc, char *argv[], t_game *game)
{
	char	*filename;
	size_t	name_len;
	int		fd;
	char	*line;

	if (argc != 2)
		exit_with_err("execute with one argument(.cub file)", E_INVAL);
	filename = get_filename_from_path(argv[1]);
	name_len = ft_strlen(filename);
	if (name_len > 4 && ft_strcmp(&filename[name_len - 4], ".cub"))
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == ERROR)
			exit_with_err("fail to open file", E_PERM);
		check_cub_elements(fd, game);
	}
	else
		exit_with_err("argument must be .cub file", E_INVAL);
}
