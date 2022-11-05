# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaesjeon <jaesjeon@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/03 12:03:16 by jaesjeon          #+#    #+#              #
#    Updated: 2022/11/05 12:33:16 by jaemjeon         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
NAME = cub3d
CFLAGS = -Wall -Wextra -Werror
CFLAGS = #-fsanitize=address -g3
MLX_DIR = mlx
LFLAGS = -L${MLX_DIR} -lmlx -framework OpenGL -framework AppKit
IFLAGS = -I${MLX_DIR} -I. -I..
MLX_LIB = libmlx.dylib
SRCS	= main.c	\
			handle_player.c	\
			make_ceiling_floor_image.c	\
			print_functions.c	\
			check_argv.c	\
			string_utils.c	\
			get_next_line.c	\
			get_next_line_utils.c	\
			utils_1.c	\
			utils_2.c	\
			check_map_elements.c	\
			check_map_elements_utils.c	\
			check_texture_elements.c	\
			check_texture_elements_utils.c	\
			rgb_utils.c	\
			make_minimap_image.c	\
			draw_minimap.c	\
			degree_utils.c	\
			ray_casting.c	\
			setting_ray.c	\
			calcul_ray.c	\
			draw_pixel.c	\
			display_texture.c	\
			control_player.c	\
			init.c	\
			key_control.c

OBJS = ${SRCS:.c=.o}

%.o:%.c
	$(CC) $(IFLAGS) $(CFLAGS) -o $@ -c $<

all : ${NAME}

$(NAME) : ${OBJS} ${MLX_LIB}
	${CC} ${CFLAGS} -o ${NAME} ${OBJS} ${LFLAGS}
	install_name_tool -change ${MLX_LIB} $(CURDIR)/${MLX_DIR}/${MLX_LIB} ${NAME}

m1 :
	arch -x86_64 make all

$(MLX_LIB) :
	make -C ${MLX_DIR} all

clean :
	make -C ${MLX_DIR} clean
	rm -f ${OBJS}

fclean : clean
	rm -f ${NAME}

.PHONY : all clean fclean bonus re

re : fclean all
