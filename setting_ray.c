#include "cub3d.h"

void	set_ray_step(t_vector2 *vec_pos, t_ray *ray, t_vector2 *step)
{
	{
		if (ray->cast_angle < 90 || ray->cast_angle >= 270)
			step->x = 1;
		else
			step->x = -1;
		if (ray->cast_angle < 180)
			step->y = 1;
		else
			step->y = -1;
	}
	{
		ray->hit_idx_x = floor(vec_pos->x);
		ray->hit_idx_y = floor(vec_pos->y);
	}
}

void	set_dist(t_vector2 *pos, double cast_angle, \
								t_vector2 *side_dist, t_vector2 *delta_dist)
{
	const double	radian_cast_angle = deg2rad(cast_angle);

	delta_dist->x = 1 / fabs(cos(radian_cast_angle));
	delta_dist->y = 1 / fabs(sin(radian_cast_angle));
	if (cos(radian_cast_angle) >= 0)
		side_dist->x = delta_dist->x * (ceil(pos->x) - pos->x);
	else
		side_dist->x = delta_dist->x * (pos->x - floor(pos->x));
	if (sin(radian_cast_angle) >= 0)
		side_dist->y = delta_dist->y * (ceil(pos->y) - pos->y);
	else
		side_dist->y = delta_dist->y * (pos->y - floor(pos->y));
}

double	__get_abs_delta_len(t_vector2 *cast_pos, t_ray *ray)
{
	if (ray->hit_wall_side == EA)
		return (sqrt(pow(ray->ray_length, 2) - \
									pow(cast_pos->x - ray->hit_point.x, 2)));
	else if (ray->hit_wall_side == WE)
		return (sqrt(pow(ray->ray_length, 2) - \
									pow(ray->hit_point.x - cast_pos->x, 2)));
	else if (ray->hit_wall_side == SO)
		return (sqrt(pow(ray->ray_length, 2) - \
									pow(cast_pos->y - ray->hit_point.y, 2)));
	else
		return (sqrt(pow(ray->ray_length, 2) - \
									pow(ray->hit_point.y - cast_pos->y, 2)));
}

void	set_ray_hit_point(t_vector2 *cast_pos, t_ray *ray)
{
	double	abs_delta_len;

	if (ray->hit_wall_side == EA)
		ray->hit_point.x = ray->hit_idx_x + 1;
	else if (ray->hit_wall_side == WE)
		ray->hit_point.x = ray->hit_idx_x;
	else if (ray->hit_wall_side == SO)
		ray->hit_point.y = ray->hit_idx_y + 1;
	else
		ray->hit_point.y = ray->hit_idx_y;
	abs_delta_len = __get_abs_delta_len(cast_pos, ray);
	if (ray->hit_wall_side == SO || ray->hit_wall_side == NO)
	{
		if (cos(deg2rad(ray->cast_angle)) > 0)
			ray->hit_point.x = cast_pos->x + abs_delta_len;
		else
			ray->hit_point.x = cast_pos->x - abs_delta_len;
	}
	else
	{
		if (sin(deg2rad(ray->cast_angle)) > 0)
			ray->hit_point.y = cast_pos->y + abs_delta_len;
		else
			ray->hit_point.y = cast_pos->y - abs_delta_len;
	}
}

void	set_wall_distance(t_player *player_info, t_ray *ray)
{
	double			delta_angle;

	delta_angle = fabs(player_info->camera_angle_h - ray->cast_angle);
	ray->wall_distance = cos(deg2rad(delta_angle)) * ray->ray_length;
}
