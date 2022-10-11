#include "cub3d.h"
#include "ft_string.h"

static void	_check_map_elements_flag(char *line, unsigned int *elements_flag)
{
	while (*line)
	{
		if (*line == 'E' || *line == 'W' || *line == 'S' || *line == 'N')
		{
			if (*elements_flag & PLAYER)
				exit_with_err("duplicated player startpoint", E_PERM);
			*elements_flag |= PLAYER;
		}
		else if (*line == '0' || *line == '1' || *line == ' ')
			;
		else
			exit_with_err("invalid map element", E_PERM);
		line++;
	}
}

void	read_map(int fd, t_info *info)
{
	unsigned int	elements_flag;
	char			*line;
	t_linked_map	*linked_map;

	line = get_map_first_line(fd);
	elements_flag = WALL;
	linked_map = NULL;
	while (line)
	{
		remove_newline(line);
		_check_map_elements_flag(line, &elements_flag);
		info->map_y++;
		linked_map = add_line_to_list(linked_map, line);
		if (info->map_x < ft_strlen(line))
			info->map_x = ft_strlen(line);
		line = get_next_line(fd);
	}
	if (elements_flag != ALL_MAP_ELEMENTS)
		exit_with_err("missing map element", E_PERM);
	linked_map_to_arr(linked_map, info);
}

void	check_map_surrounded_by_wall(char **map, int end_x, int end_y)
{
	int	x;
	int	y;

	y = 0;
	while (y < end_y)
	{
		x = 0;
		while (x < end_x)
		{
			if (map[y][x] == '0' || map[y][x] == 'E' || map[y][x] == 'W' \
									|| map[y][x] == 'S' || map[y][x] == 'N')
			{
				if (x == 0 || x == end_x - 1 || y == 0 || y == end_y - 1)
					exit_with_err("map is not surrounded by wall", E_PERM);
				else if (map[y][x - 1] == ' ' || map[y][x + 1] == ' ' \
							|| map[y - 1][x] == ' ' || map[y + 1][x] == ' ')
					exit_with_err("map is not surrounded by wall", E_PERM);
			}
			x++;
		}
		y++;
	}
}
