#include "cub3d.h"
#include "mlx.h"
#include "ft_string.h"

enum	e_rgb
{
	R = 0,
	G,
	B
};

static void	_set_wall_texture(t_game *game, int element_identifier, \
												char *texture_file)
{
	t_texture	*target;
	t_img_data	*target_img;

	target = &game->texture[element_identifier];
	if (target->filename)
		free(target->filename);
	target->filename = ft_strcpy(texture_file);
	if (target->img_data.img)
		mlx_destroy_image(game->info.mlx_ptr, target->img_data.img);
	target->img_data.img = mlx_xpm_file_to_image(game->info.mlx_ptr, \
				texture_file, &target->texture_width, &target->texture_height);
	if (target->img_data.img == NULL)
		exit_with_err("can't open texture file", E_PERM);
	target->img_data.addr = mlx_get_data_addr(target->img_data.img, \
		&target->img_data.bits_per_pixel, &target->img_data.line_length, \
		&target->img_data.endian);
}

static void	_set_floor_ceiling_color(t_game *game, int element_identifier, \
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
		if (idx <=B && *rgb_str == ',')
			rgb_str++;
	}
	while (*rgb_str && (*rgb_str == ' ' || *rgb_str == '\t'))
		rgb_str++;
	if (*rgb_str != '\0')
		exit_with_err("floor, ceiling color value error", E_PERM);
	*target = create_trgb(0, color_value[R], color_value[G], color_value[B]);
}

static void	_get_texture_elements_info(int fd, \
									unsigned char *elements_flag, t_game *game)
{
	char	*line;
	char	*pair[2];
	int		element_identifier;

	while (*elements_flag != ALL_TEXTURE_ELEMENTS)
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
		element_identifier = get_texture_element_identifier(pair[KEY]);
		if (element_identifier == ERROR || pair[VALUE] == NULL)
			exit_with_err("invalid element source", E_PERM);
		else if (element_identifier < F)
			_set_wall_texture(game, element_identifier, pair[VALUE]);
		else
			_set_floor_ceiling_color(game, element_identifier, pair[VALUE]);
		*elements_flag |= get_texture_elements_flag_bit(element_identifier);
		multi_free(pair[KEY], pair[VALUE], line, NULL);
	}
}

void	check_texture_elements(int fd, t_game *game)
{
	unsigned char	elements_flag;

	elements_flag = 0;
	_get_texture_elements_info(fd, &elements_flag, game);
	if (elements_flag != ALL_TEXTURE_ELEMENTS)
		exit_with_err("invalid element source", E_PERM);
}
