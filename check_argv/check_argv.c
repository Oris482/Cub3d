#include "../cub3d.h"
#include "../mlx/mlx.h"
#include <fcntl.h>

#define ERROR -1

#define KEY	0
#define VALUE 1

enum	e_cub_elements
{
	EA_TEXTURE = 1 << 0,
	WE_TEXTURE = 1 << 1,
	SO_TEXTURE = 1 << 2,
	NO_TEXTURE = 1 << 3,
	FLOOR_COLOR = 1 << 4,
	CEILING_COLOR = 1 << 5,
	WALL = 1 << 6,
	PLAYER = 1 << 7
};

enum	e_rgb
{
	R = 0,
	G,
	B
};

void	exit_with_err(char *custom_msg, int exit_code);
size_t	ft_strlen(const char *s);
char	*ft_strchr(const char *s, const char c);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_substr(const char *start, const char *end);
char	*ft_strcpy(const char *s);
char	*get_next_line(int fd);
int		create_trgb(unsigned char t, unsigned char r, \
								unsigned char g, unsigned char b);
unsigned char	get_r(int trgb);
unsigned char	get_g(int trgb);
unsigned char	get_b(int trgb);


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

int	get_element_identifier(char *key)
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

void	set_wall_texture(t_game *game, int element_identifier, \
												char *texture_file)
{
	t_texture	*target;

	target = &game->texture[element_identifier];
	target->filename = ft_strcpy(texture_file);
	target->img = mlx_xpm_file_to_image(game->info.mlx_ptr, texture_file, \
								&target->texture_width, &target->texture_height);
	if (target->img == NULL)
		exit_with_err("can't open texture file", E_PERM);
}

unsigned char	get_color_value(char **rgb_str)
{
	int	color_value;

	color_value = 0;
	while (**rgb_str && **rgb_str != ',')
	{
		if (**rgb_str < '0' || **rgb_str > '9')
			exit_with_err("floor, ceiling color value error", E_PERM);
		color_value = color_value * 10 + (**rgb_str - '0');
		if (color_value > 255)
			exit_with_err("floor, ceiling color value error", E_PERM);
		(*rgb_str)++;
	}
	return ((unsigned char)color_value);
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

unsigned char	get_elements_flag_bit(int element_identifier)
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

void	get_elements_info(int fd, unsigned char *elements_flag, t_game *game)
{
	char	*line;
	char	*pair[2];
	int		element_identifier;

	while (1)
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
		else if (*elements_flag & get_elements_flag_bit(element_identifier))
			exit_with_err("duplicate element", E_PERM);
		else if (element_identifier < F)
			set_wall_texture(game, element_identifier, pair[VALUE]);
		else
			set_floor_ceiling_color(game, element_identifier, pair[VALUE]);
		*elements_flag |= get_elements_flag_bit(element_identifier);
		free(pair[KEY]);
		free(pair[VALUE]);
		free(line);
		if (*elements_flag == 0b00111111)
			return ;
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
	printf("texture_ea: %s\naddress : %p | width/height : %d/%d\n", game->texture[EA].filename, game->texture[EA].img, game->texture[EA].texture_width, game->texture[EA].texture_height);
	printf("texture_we: %s\naddress : %p | width/height : %d/%d\n", game->texture[WE].filename, game->texture[WE].img, game->texture[WE].texture_width, game->texture[WE].texture_height);
	printf("texture_so: %s\naddress : %p | width/height : %d/%d\n", game->texture[SO].filename, game->texture[SO].img, game->texture[SO].texture_width, game->texture[SO].texture_height);
	printf("texture_no: %s\naddress : %p | width/height : %d/%d\n", game->texture[NO].filename, game->texture[NO].img, game->texture[NO].texture_width, game->texture[NO].texture_height);
	printf("floor_color: R %d G %d B %d\n", get_r(game->floor_color), get_g(game->floor_color), get_b(game->floor_color));
	printf("ceiling_color: R %d G %d B %d\n", get_r(game->ceiling_color), get_g(game->ceiling_color), get_b(game->ceiling_color));
}
