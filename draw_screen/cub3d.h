#ifndef CUB3D_H
# define CUB3D_H

#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "mlx/mlx.h"
#define PI M_PI
#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define HALF_VIEW_ANGLE 30
#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT SCREEN_WIDTH * 0.75

enum e_direction
{
	NO = 1 << 0,
	SO = 1 << 1,
	EA = 1 << 2,
	WE = 1 << 3
};

typedef struct s_vector2
{
	double	vec_x;
	double	vec_y;
}	t_vector2;

typedef struct s_ray_spot
{
	t_vector2	vec_hit_point;
	double		distance;
	int			hit_side;
}	t_ray_spot;

typedef struct s_ray
{
	double		cast_angle;
	t_ray_spot	hit_data;
}	t_ray;

typedef struct s_player
{
	t_vector2	vec_pos;
	double		camera_angle;
}	t_player;

#endif