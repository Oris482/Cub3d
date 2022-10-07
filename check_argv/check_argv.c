#include "../cub3d.h"

void	exit_with_err(char *custom_msg, int exit_code);
size_t	ft_strlen(const char *s);
int	ft_strcmp(const char *s1, const char *s2);

void	check_argv(int argc, char *argv[])
{
	const char	*filename = argv[1];
	size_t		name_len;

	if (argc != 2)
		exit_with_err("excute with one argument(.cub file)", 1);
	name_len = ft_strlen(filename);
	if (name_len > 4 && ft_strcmp(&filename[name_len - 4], ".cub"))
		return ;
	exit_with_err("argument must be .cub file", 1);
}
