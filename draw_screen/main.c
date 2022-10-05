#include "./cub3d.h"

void	init_player(t_player *player, int argc, char *argv[])
{
	int error_flag = 0;

	if (argc != 4)
	{
		printf("x좌표, y좌표, 보는 방향..3가지 인자를 넣어주세요\n");
		exit(1);
	}
	player->vec_pos.vec_x = atof(argv[1]);
	player->vec_pos.vec_y = atof(argv[2]);
	player->camera_angle = atof(argv[3]); // 라디안이 들어감 0 ~ 2PI
	if (1.5 > player->vec_pos.vec_x && player || player->vec_pos.vec_x > 8.5)
	{
		error_flag = 1;
		printf("x좌표 오류\n");
	}
	if (1.5 > player->vec_pos.vec_y && player || player->vec_pos.vec_y > 8.5)
	{
		error_flag = 1;
		printf("y좌표 오류\n");
	}
	if (error_flag)
		exit(1);
	printf("%lf, %lf, %lf", player->vec_pos.vec_x, player->vec_pos.vec_y, player->camera_angle);
}

void	init_graphic_resource(void **mlx_ptr, void **win_ptr, t_ray **ray)
{
	*mlx_ptr = mlx_init();
	if (*mlx_ptr == NULL)
	{
		printf("mlx_init fail");
		exit(1);
	}
	*win_ptr = mlx_new_window(*mlx_ptr, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3d");
	if (*win_ptr == NULL)
	{
		printf("mlx_new_window fail");
		exit(1);
	}
	*ray = calloc(SCREEN_WIDTH, sizeof(t_ray));
	if (*ray == NULL)
	{
		perror(strerror(errno));
		exit(errno);
	}
}

void	cast_single_ray(t_player *player, int index, t_ray_spot *ray_spot)
{

}

void	ray_cast(t_player *player, t_ray_spot *ray_spot)
{
	int	index;

	index = 0;
	while (index < SCREEN_WIDTH)
	{
		cast_single_ray(player, index, &ray_spot[index]);
		/*
		여기에서 인덱스에 해당하는 각도로 빛을 쏴 마지막 인자의 구조체에 저장함.
		빛을 맞은 좌표, 카메라평면과의 거리, 어느면에 맞았는지 정보를 저장해줌.
		*/
		index++;
	}
}

void	*get_frame_img(t_player *player, t_ray_spot *ray_spot, void *mlx_ptr)
{
	void	*frame_img;
	int		index;

	index = 0;
	frame_img = mlx_new_image(mlx_ptr, SCREEN_WIDTH, SCREEN_HEIGHT);
	while (index < SCREEN_WIDTH)
	{
		// 여기서 한 빛에 대한 정보를 기준으로 이미지의 한 x값에대한 y들의 색을 넣어주는 작업을 해야함
		index++;
	}

	return (frame_img);
}

void	render(t_player *player, t_ray_spot *ray_spot, void *mlx_ptr, void *win_ptr)
{
	void	*frame_img;

	ray_cast(player, ray_spot);
	/*
	여기서 현재위치와 바라보는 각도를 기준으로 가로 픽셀수만큼 등분된 ray를 던져서 벽에 맞으면
	그 정보를 ray_spot배열에 차곡차곡 저장할것임
	*/
	frame_img = get_frame_img(player, ray_spot, mlx_ptr);
	/*
	ray_casting으로 얻은 정보로 픽셀을 하나씩 씩고 벽을 기준으로(상하로 시야조정할 수 있게 되면 다르게 해야겠지만)
	나머지 픽셀에 천장과 바닥의 색을 칠해하는 방식으로 한 프레임의 이미지를 만든다.
	*/
	mlx_put_image_to_window(mlx_ptr, win_ptr, frame_img, 0, 0);
	mlx_destroy_image(mlx_ptr, frame_img);
	/*
	한 프레임을 화면에 출력후 할당받은 이미지정보를 지워버린다.
	*/
}

int	main(int argc, char *argv[])
{
	t_player	player;
	void		*mlx_ptr;
	void		*win_ptr;
	t_ray		*ray;
	// 맵은 벽포함 10 X 10의 사이즈로 가정함
	static char	*map[] = {
		"1111111111",
		"1000000001",
		"1000000001",
		"1000000001",
		"1000000001",
		"1000000001",
		"1000000001",
		"1000000001",
		"1000000001",
		"1111111111"
	};
	init_player(&player, argc, argv);
	init_graphic_resource(&mlx_ptr, &win_ptr, &ray);
	render(&player, ray, mlx_ptr, win_ptr);
	mlx_loop(mlx_ptr);
}
