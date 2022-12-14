/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaemjeon <jaemjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 16:03:19 by jaemjeon          #+#    #+#             */
/*   Updated: 2022/11/13 10:02:47 by jaemjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdlib.h>

# define KEY_PRESS_EVENT 2
# define KEY_RELEASE_EVENT 3

# define ERROR -1
# define SUCCESS 1
# define FAIL 0

# define TRUE 1
# define FALSE 0

# define START	0
# define END	1

# define KEY	0
# define VALUE 1

# define X	0
# define Y	1

# define NONE ' '
# define WALL '1'
# define RODE '0'

# define BODY_SIDE_2 0.1
# define SPEED_MOUSE_H 0.1
# define SPEED_MOUSE_V 2

enum	e_keyset
{
	KEYSET_W = 1,
	KEYSET_A = 1 << 1,
	KEYSET_S = 1 << 2,
	KEYSET_D = 1 << 3,
	KEYSET_LA = 1 << 4,
	KEYSET_RA = 1 << 5,
	KEYSET_DA = 1 << 6,
	KEYSET_UA = 1 << 7,
	KEYSET_LSHIFT = 1 << 8,
	KEY_WASD = KEYSET_W | KEYSET_A | KEYSET_S | KEYSET_D,
	KEY_ARROW = KEYSET_LA | KEYSET_RA | KEYSET_DA | KEYSET_UA
};

enum	e_key_code
{
	KEY_A = 0,
	KEY_S = 1,
	KEY_D = 2,
	KEY_W = 13,
	KEY_H = 4,
	KEY_J = 38,
	KEY_K = 40,
	KEY_L = 37,
	KEY_M = 46,
	KEY_EXIT = 17,
	KEY_ESC = 53,
	KEY_LSHIFT = 257,
	KEY_LA = 123,
	KEY_RA = 124,
	KEY_DA = 125,
	KEY_UA = 126
};

enum	e_texture_identifier
{
	EA = 0,
	WE,
	SO,
	NO,
	F,
	C
};

enum	e_texture_elements
{
	EA_TEXTURE = 1 << 0,
	WE_TEXTURE = 1 << 1,
	SO_TEXTURE = 1 << 2,
	NO_TEXTURE = 1 << 3,
	FLOOR_COLOR = 1 << 4,
	CEILING_COLOR = 1 << 5,
	ALL_TEXTURE_ELEMENTS = EA_TEXTURE | WE_TEXTURE | SO_TEXTURE | NO_TEXTURE \
							| FLOOR_COLOR | CEILING_COLOR
};

enum	e_map_elements
{
	WALL_ELEMENT = 1 << 0,
	PLAYER_ELEMENT = 1 << 1,
	ALL_MAP_ELEMENTS = WALL_ELEMENT | PLAYER_ELEMENT
};

enum	e_exit_status
{
	E_PERM = 1,
	E_NOMEM = 12,
	E_INVAL = 22,
};

typedef struct s_linked_map
{
	char				*line;
	struct s_linked_map	*next;
}	t_linked_map;

typedef struct s_outside_wall_info
{
	int	x;
	int	range_y[2];
}	t_outside_wall_info;

typedef struct s_vector2
{
	double	x;
	double	y;
}	t_vector2;

typedef struct s_vector2_d
{
	int	x;
	int	y;
}	t_vector2_d;

typedef struct s_info
{
	void	*mlx_ptr;
	void	*win_ptr;
	int		screen_x;
	int		screen_y;
	double	fov_h;
	double	fov_v;
	char	**map;
	int		map_x;
	int		map_y;
	int		using_mouse;
}	t_info;

typedef struct s_player
{
	t_vector2		vec_pos;
	double			camera_angle_h;
	double			vertical_dist_pixel;
	double			move_speed;
	double			rotate_speed_h;
	int				rotate_speed_v;
	int				view_trans;
}	t_player;

typedef struct s_img_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_img_data;

typedef struct s_texture
{
	t_img_data	img_data;
	char		*filename;
	int			texture_width;
	int			texture_height;
}	t_texture;

typedef struct s_ray
{
	double		cast_angle;
	double		ray_length;
	double		wall_distance;
	t_vector2	hit_point;
	int			hit_idx_x;
	int			hit_idx_y;
	int			hit_wall_side;
	double		hit_texture_point;
}	t_ray;

typedef struct s_minimap
{
	t_img_data	map_img_data;
	int			width;
	int			height;
	int			pixel_per_square;
}	t_minimap;

typedef struct s_game
{
	t_info			info;
	t_player		player;
	t_img_data		view_data;
	t_minimap		minimap;
	t_texture		texture[4];
	t_ray			*ray_data;
	t_vector2_d		*wall_pixel;
	t_vector2		*wall_line;
	unsigned int	pressed_keyset;
	unsigned int	floor_color;
	unsigned int	ceiling_color;
}	t_game;

// utils.c
void			multi_free(void *ptr1, void *ptr2, void *ptr3, void *ptr4);
void			*my_malloc(size_t len);
void			ft_memset(void *ptr, unsigned char value, size_t size);
void			remove_newline(char *line);
void			set_range_int(int *to_set, int floor, int ceil);
void			set_range_double(double *to_set, double floor, double ceil);
int				exit_game(int exit_status);

// degree_utils.c
double			deg2rad(double degree);
double			adjust_degree(double base_degree, double offset_degree);

// print_functions.c
void			exit_with_err(char *custom_msg, int exit_code);
void			print_game_info(t_game *game);
void			print_map(char **map, int end_x, int end_y);

// rgb_utils.c
int				create_trgb(unsigned char t, unsigned char r, \
								unsigned char g, unsigned char b);
unsigned char	get_r(int trgb);
unsigned char	get_g(int trgb);
unsigned char	get_b(int trgb);

// make_ceiling_floor_image.c
void			set_range(int arg[2], int arg_start, int arg_end);
void			draw_ceiling_floor(t_game *game, int x, int wall_y[2], \
															double fog_value);

// check_texture_elements_utils.c
char			*get_filename_from_path(char *filename);
unsigned char	get_texture_elements_flag_bit(int element_identifier);
int				get_texture_element_identifier(char *key);
void			seperate_by_key_value(char **pair, char *line);
unsigned char	get_color_value(char **rgb_str);

// check_texture_elements.c
void			check_texture_elements(int fd, t_game *game);

// check_map_elements_utils.c
char			*get_map_first_line(int fd);
t_linked_map	*add_line_to_list(t_linked_map *linked_map, char *line);
void			linked_map_to_arr(t_linked_map *linked_map, t_info *info);

// check_map_elements.c
void			read_map(int fd, t_info *info, t_player *player);
void			check_map_surrounded_by_wall(char **map, int end_x, int end_y);

// check_argv.c
void			check_argv(int argc, char *argv[], t_game *game);

// ray_casting.c
void			init_ray(t_game *game);
void			draw_screen(t_game *game);

// handle_player.c
double			cut_point(double num, int limiter);
void			move_player(t_player *player, char **map, \
												unsigned int pressed_keyset);
void			rotate_player_key(t_game *game, \
											unsigned int const pressed_keyset);
void			rotate_player_mouse(t_game *game);

// make_minimap_imgae.c
void			put_one_square_pixels_to_img(t_img_data *img_data, int pos[2], \
												int pps, unsigned int color);
void			make_minimap_image(t_game *game);

// draw_minimap.c
void			draw_minimap(t_game *game);

// display_texture.c
void			put_pixel_wall(t_game *game, int idx_x);

// setting_ray_1.c
void			set_ray_step(t_vector2 *vec_pos, t_ray *ray, t_vector2 *step);
void			set_dist(t_vector2 *pos, double cast_angle, \
								t_vector2 *side_dist, t_vector2 *delta_dist);
void			set_ray_hit_point(t_vector2 *cast_pos, t_ray *ray);
void			set_wall_distance(t_player *player_info, t_ray *ray);

// calcul_ray.c
void			calcul_texture_x_point(t_ray *ray);
void			calcul_hitpoint_dist(t_ray *ray, int last_step, \
								t_vector2 *side_dist, t_vector2 *delta_dist);
void			calcul_drawpixel(t_game *game, t_ray *ray, int idx_x);
void			get_hit_wall_side(t_ray *ray, int last_step);
int				ray_travel(t_ray *ray, t_vector2 *step, \
					t_vector2 *side_dist, t_vector2 *delta_dist);

// draw_pixel.c
void			put_pixel_floor(t_game *game, int idx_x);
void			put_pixel_ceiling(t_game *game, int idx_x);
t_vector2		get_wall_pixel(t_game *game, t_vector2 *wall_line);
void			draw_line(t_game *game, int idx_x);

// control_player.c
void			turn_cardinal_points(int key, t_player *player);
void			boost_key_off(t_game *game);
void			boost_key_on(t_game *game);
int				mouse_switch(int key_code, t_game *game);

// init.c
void			malloc_resources(t_game *game);
void			graphic_resource_init(t_game *game);
void			player_handle_setting(t_game *game);

// key_control.c
int				ft_key_press(int key, t_game *game);
int				ft_key_release(int key, t_game *game);

#endif
