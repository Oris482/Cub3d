#include "../cub3d.h"
#include "ft_string.h"

char	*get_filename_from_path(char *filename)
{
	while (1)
	{
		if (ft_strchr(filename, '/') == NULL)
			break ;
		filename = ft_strchr(filename, '/') + 1;
	}
	return (filename);
}

unsigned char	get_texture_elements_flag_bit(int element_identifier)
{
	if (element_identifier == EA)
		return (EA_TEXTURE);
	else if (element_identifier == WE)
		return (WE_TEXTURE);
	else if (element_identifier == SO)
		return (SO_TEXTURE);
	else if (element_identifier == NO)
		return (NO_TEXTURE);
	else if (element_identifier == F)
		return (FLOOR_COLOR);
	else if (element_identifier == C)
		return (CEILING_COLOR);
	else
		return (ERROR);
}

int	get_texture_element_identifier(char *key)
{
	if (ft_strcmp(key, "EA"))
		return (EA);
	else if (ft_strcmp(key, "WE"))
		return (WE);
	else if (ft_strcmp(key, "SO"))
		return (SO);
	else if (ft_strcmp(key, "NO"))
		return (NO);
	else if (ft_strcmp(key, "F"))
		return (F);
	else if (ft_strcmp(key, "C"))
		return (C);
	else
		return (ERROR);
}

void	seperate_by_key_value(char **pair, char *line)
{
	char	*start;

	while (*line && (*line == ' ' || *line == '\t' || *line == '\n'))
		line++;
	start = line;
	while (*line && (*line != ' ' && *line != '\t'))
		line++;
	pair[KEY] = ft_substr(start, line);
	while (*line && (*line == ' ' || *line == '\t' || *line == '\n'))
		line++;
	start = line;
	while (*line != '\0' && *line != '\n')
		line++;
	pair[VALUE] = ft_substr(start, line);
}

unsigned char	get_color_value(char **rgb_str)
{
	int	color_value;

	color_value = -1;
	while (**rgb_str && **rgb_str != ',')
	{
		if (**rgb_str < '0' || **rgb_str > '9')
			exit_with_err("floor, ceiling color value error", E_PERM);
		if (color_value == -1)
			color_value = 0;
		color_value = color_value * 10 + (**rgb_str - '0');
		if (color_value > 255)
			exit_with_err("floor, ceiling color value error", E_PERM);
		(*rgb_str)++;
	}
	if (color_value < 0)
		exit_with_err("floor, ceiling color value error", E_PERM);
	return ((unsigned char)color_value);
}
