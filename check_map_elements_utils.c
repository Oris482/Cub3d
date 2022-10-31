#include "cub3d.h"
#include "ft_string.h"

char	*get_map_first_line(int fd)
{
	char	*line;
	char	*cursor;

	while (TRUE)
	{
		line = get_next_line(fd);
		if (line == NULL)
			exit_with_err("invalid map", E_PERM);
		cursor = line;
		while (*cursor && (*cursor == NONE || *cursor == '\n'))
			cursor++;
		if (*cursor == '\0')
		{
			free(line);
			continue ;
		}
		else if (*cursor != WALL)
			exit_with_err("invalid map", E_PERM);
		while (*cursor && \
						(*cursor == WALL || *cursor == NONE || *cursor == '\n'))
			cursor++;
		if (*cursor == '\0')
			return (line);
		exit_with_err("invalid map", E_PERM);
	}
}

t_linked_map	*add_line_to_list(t_linked_map *linked_map, char *line)
{
	const t_linked_map	*head = linked_map;

	if (linked_map == NULL)
	{
		linked_map = (t_linked_map *)my_malloc(sizeof(t_linked_map));
		linked_map->line = line;
		linked_map->next = NULL;
		return (linked_map);
	}
	else
	{
		while (linked_map->next)
			linked_map = linked_map->next;
		linked_map->next = (t_linked_map *)my_malloc(sizeof(t_linked_map));
		linked_map->next->line = line;
		linked_map->next->next = NULL;
		return ((t_linked_map *)head);
	}
}

void	linked_map_to_arr(t_linked_map *linked_map, t_info *info)
{
	int				x;
	int				y;
	t_linked_map	*tmp;

	if (info->map_y < 3)
		exit_with_err("invalid map", E_PERM);
	info->map = (char **)my_malloc(sizeof(char *) * info->map_y);
	y = 0;
	while (linked_map)
	{
		info->map[y] = (char *)my_malloc(sizeof(char) * info->map_x);
		ft_memset(info->map[y], NONE, info->map_x);
		x = 0;
		while (linked_map->line[x])
		{
			info->map[y][x] = linked_map->line[x];
			x++;
		}
		tmp = linked_map;
		linked_map = linked_map->next;
		multi_free(tmp->line, tmp, NULL, NULL);
		y++;
	}
}
