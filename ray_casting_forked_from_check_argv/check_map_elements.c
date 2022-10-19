#include "cub3d.h"
#include "ft_string.h"

static void	_set_init_player(char c, t_player *player, int map_x, int map_y)
{
	player->vec_pos.x = map_x + 0.5;
	player->vec_pos.y = map_y + 0.5;
	if (c == 'E')
		player->camera_angle = 0.0;
	else if (c == 'W')
		player->camera_angle = 180.0;
	else if (c == 'S')
		player->camera_angle = 90.0;
	else if (c == 'N')
		player->camera_angle = 270.0;
}

static void	_check_map_elements_flag(char *line, unsigned int *elements_flag, \
													t_player *player, int map_y)
{
	int	map_x;

	map_x = 0;
	while (line[map_x])
	{
		if (line[map_x] == 'E' || line[map_x] == 'W' \
			|| line[map_x] == 'S' || line[map_x] == 'N')
		{
			if (*elements_flag & PLAYER)
				exit_with_err("duplicated player startpoint", E_PERM);
			_set_init_player(line[map_x], player, map_x, map_y);
			*elements_flag |= PLAYER;
		}
		else if (line[map_x] == '0' || line[map_x] == '1' || line[map_x] == ' ')
			;
		else
			exit_with_err("invalid map element", E_PERM);
		map_x++;
	}
}

void	read_map(int fd, t_info *info, t_player *player)
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
		_check_map_elements_flag(line, &elements_flag, player, info->map_y);
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
