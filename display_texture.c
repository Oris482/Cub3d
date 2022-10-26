#include "cub3d.h"

unsigned int	get_texture_pixel(t_game *game, int idx_x, \
										t_vector2 *wall_range, int cur_idx_y)
{
	t_ray * const	cur_ray = &game->ray_data[idx_x];
	t_img_data		*texture_img;
	t_vector2		ratio_pos_texture;
	int				texture_idx_x;
	int				texture_idx_y;

	texture_img = &game->texture[cur_ray->hit_wall_side].img_data;
	texture_idx_x = game->texture[cur_ray->hit_wall_side].texture_width;
	texture_idx_y = game->texture[cur_ray->hit_wall_side].texture_height;
	ratio_pos_texture.x = cur_ray->hit_texture_point;
	ratio_pos_texture.y = \
				(cur_idx_y - wall_range->x) / (wall_range->y - wall_range->x);
	texture_idx_x *= ratio_pos_texture.x;
	texture_idx_y *= ratio_pos_texture.y;
	return (*(unsigned int *)(texture_img->addr + \
			texture_idx_y * texture_img->line_length + texture_idx_x * \
											(texture_img->bits_per_pixel / 8)));
}

void	put_pixel_wall(t_game *game, int idx_x)
{
	t_img_data * const	view_data = &game->view_data;
	double 				gradiant = 1 - game->ray_data[idx_x].ray_length * 0.07;
	double 				color;
	char				*dst;
	int					cur_idx_y;

	if (gradiant > 1)
		gradiant = 1;
	else if (gradiant < 0)
		gradiant = 0;
	cur_idx_y = game->wall_pixel[idx_x].x;
	while (cur_idx_y < game->wall_pixel[idx_x].y)
	{
		dst = view_data->addr + (cur_idx_y * view_data->line_length + \
								idx_x * (view_data->bits_per_pixel / 8));
		color = get_texture_pixel(game, idx_x, wall_range, cur_idx_y);
		*(unsigned int *)dst = create_trgb(0, get_r(color) * gradiant, \
							get_g(color) * gradiant, get_b(color) * gradiant);
		cur_idx_y++;
	}
}