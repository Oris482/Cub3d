#include "cub3d.h"
#include "mlx.h"
#include <fcntl.h>
#include "ft_string.h"

typedef struct s_linked_map
{
	char				*line;
	struct s_linked_map	*next;
}	t_linked_map;


void	check_texture_elements(int fd, t_game *game)
{
	unsigned char	elements_flag;

	elements_flag = 0;
	get_texture_elements_info(fd, &elements_flag, game);
	if (elements_flag != ALL_TEXTURE_ELEMENTS)
		exit_with_err("some elements source missing", E_PERM);
}

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
		while (*cursor && (*cursor == ' ' || *cursor == '\n'))
			cursor++;
		if (*cursor == '\0')
		{
			free(line);
			continue ;
		}
		else if (*cursor != '1')
			exit_with_err("invalid map", E_PERM);
		while (*cursor && (*cursor == '1' || *cursor == ' ' || *cursor == '\n'))
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

void	check_map_elements_flag(char *line, unsigned int *elements_flag)
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

void	linked_map_to_arr(t_linked_map *linked_map, t_info *info)
{
	int				x;
	int				y;
	t_linked_map	*tmp;

	if (info->map_y == 0)
		exit_with_err("there is no map", E_PERM);
	printf("map_x : %d, map_y : %d\n", info->map_x, info->map_y);
	info->map = (char **)my_malloc(sizeof(char *) * info->map_y);
	y = 0;
	while (linked_map)
	{
		info->map[y] = (char *)my_malloc(sizeof(char) * info->map_x);
		ft_memset(info->map[y], ' ', info->map_x);
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
		check_map_elements_flag(line, &elements_flag);
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

void	check_argv(int argc, char *argv[], t_game *game)
{
	char	*filename;
	size_t	name_len;
	int		fd;

	if (argc != 2)
		exit_with_err("execute with one argument(.cub file)", E_INVAL);
	filename = get_filename_from_path(argv[1]);
	name_len = ft_strlen(filename);
	if (name_len > 4 && ft_strcmp(&filename[name_len - 4], ".cub"))
	{
		printf("Valid filename. Try to open file\n");
		fd = open(argv[1], O_RDONLY);
		if (fd == ERROR)
			exit_with_err("fail to open file", E_PERM);
		printf("Success to open file. Reading texture elements...\n");
		check_texture_elements(fd, game);
		printf("All texture elements loaded. Reading map...\n");
		printf("*From now on duplicated texture will be considered error\n");
		read_map(fd, &game->info);
	}
	else
		exit_with_err("argument must be .cub file", E_INVAL);
}
