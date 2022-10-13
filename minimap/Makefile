CC = cc
NAME = a.out
CFLAGS = #-Wall -Wextra -Werror
CFLAGS = -g
MLX_DIR = ../mlx
LFLAGS = -L${MLX_DIR} -lmlx -framework OpenGL -framework AppKit
IFLAGS = -I${MLX_DIR} -I. -I..
MLX_LIB = libmlx.dylib
SRCS	= main.c	\
			degree_utils.c	\
			make_ceiling_floor_image.c	\
			print_functions.c	\
			check_argv.c	\
			string_utils.c	\
			get_next_line.c	\
			get_next_line_utils.c	\
			utils.c	\
			check_map_elements.c	\
			check_map_elements_utils.c	\
			check_texture_elements.c	\
			check_texture_elements_utils.c	\
			rgb_utils.c	\
			make_minimap_image.c

OBJS = ${SRCS:.c=.o}

%.o:%.c
	$(CC) $(IFLAGS) $(CFLAGS) -o $@ -c $<

all : ${NAME}

$(NAME) : ${OBJS} ${MLX_LIB}
	${CC} ${CFLAGS} -o ${NAME} ${OBJS} ${LFLAGS}
	install_name_tool -change ${MLX_LIB} ./${MLX_DIR}/${MLX_LIB} ${NAME}

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
