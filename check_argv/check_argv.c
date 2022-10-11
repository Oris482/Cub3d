#include "cub3d.h"
#include "mlx.h"
#include <fcntl.h>
#include "ft_string.h"

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
		check_map_surrounded_by_wall(game->info.map, \
											game->info.map_x, game->info.map_y);
		printf("Valid map. Start rendering...\n");
	}
	else
		exit_with_err("argument must be .cub file", E_INVAL);
}
