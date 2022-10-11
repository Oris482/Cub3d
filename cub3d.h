#ifndef CUB3D_H
# define CUB3D_H
# define SIZE_WIN_X 400
# define SIZE_WIN_Y (SIZE_WIN_X * 3 / 4)
# define KEY_PRESS_EVENT 2
# define KEY_RELEASE_EVENT 3

# include <math.h>
# include <stdlib.h>
# include <stdio.h>

# define ERROR -1
# define SUCCESS 1
# define FALSE 0

# define KEY	0
# define VALUE 1

enum	e_keyset
{
	KEYSET_W = 1,
	KEYSET_A = 1 << 1,
	KEYSET_S = 1 << 2,
	KEYSET_D = 1 << 3,
	KEYSET_LA = 1 << 4,
	KEYSET_RA = 1 << 5,
	KEYSET_LSHIFT = 1 << 6,
	KEY_WASD = KEYSET_W | KEYSET_A | KEYSET_S | KEYSET_D,
	KEY_ARROW = KEYSET_LA | KEYSET_RA
};

enum	e_key_code
{
	KEY_A = 0,
	KEY_S = 1,
	KEY_D = 2,
	KEY_W = 13,
	KEY_EXIT = 17,
	KEY_ESC = 53,
	KEY_LSHIFT = 57,
	KEY_LA = 123,
	KEY_RA = 124
};

enum	e_element_identifier
{
	EA = 0,
	WE,
	SO,
	NO,
	F,
	C
};

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

enum	e_exit_status
{
	E_PERM = 1,
	E_INVAL = 22,
};

typedef struct s_vector2
{
	double	x;
	double	y;
}	t_vector2;

typedef struct s_info
{
	void	*mlx_ptr;
	void	*win_ptr;
	int		screen_x;
	int		screen_y;
	double	fov_h;
	double	fov_v;
	char	**map;
}	t_info;

typedef struct s_controller
{
	unsigned int	pressed_keyset;
}	t_controller;

typedef struct s_player
{
	t_vector2		vec_pos;
	double			camera_angle;
	double			move_speed;
	double			rotate_speed;
}	t_player;

typedef struct	s_bg_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}   t_bg_data;

typedef struct	s_texture {
	void	*img;
	char	*filename;
	int		texture_width;
	int		texture_height;
}	t_texture;

typedef struct s_game
{
	t_info			info;
	t_player		player;
	t_bg_data		bg_data;
	t_texture		texture[4];
	unsigned int	pressed_keyset;
	char			**map;
	unsigned int	floor_color;
	unsigned int	ceiling_color;
}	t_game;

// utils.c
void    		multi_free(void *ptr1, void *ptr2, void *ptr3, void *ptr4);
void			ft_memset(void *ptr, unsigned char value, size_t size);

// print_functions.c
void			exit_with_err(char *custom_msg, int exit_code);
void			print_game_info(t_game *game);

// rgb_utils.c
int				create_trgb(unsigned char t, unsigned char r, \
								unsigned char g, unsigned char b);
unsigned char	get_r(int trgb);
unsigned char	get_g(int trgb);
unsigned char	get_b(int trgb);

// make_ceiling_floor_image.c
void			draw_ceiling_floor(t_game *game, int x, int y_top, int y_bottom, double fog_value);

// check_element_utils.c
char			*get_filename_from_path(char *filename);
unsigned char	get_elements_flag_bit(int element_identifier);
int				get_element_identifier(char *key);
void			seperate_by_key_value(char **pair, char *line);
unsigned char	get_color_value(char **rgb_str);

// check_argv.c
void			check_argv(int argc, char *argv[], t_game *game);

#endif
