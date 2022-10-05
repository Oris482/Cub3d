#ifndef CUB3D_H
# define CUB3D_H
# define SIZE_WIN_X 400
# define SIZE_WIN_Y (SIZE_WIN_X * 3 / 4)
# define KEY_PRESS_EVENT 2
# define KEY_RELEASE_EVENT 3

# include <math.h>

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

typedef struct s_game
{
	t_info			info;
	t_player		player;
	unsigned int	pressed_keyset;
	char			**map;
	void			*texture[4];
	unsigned int	floor_color;
	unsigned int	ceilling_color;
}	t_game;

#endif
