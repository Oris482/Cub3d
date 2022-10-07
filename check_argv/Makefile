CC = cc
NAME = a.out
CFLAGS = #-Wall -Wextra -Werror
MLX_DIR = ../mlx
LFLAGS = -L${MLX_DIR} -lmlx -framework OpenGL -framework AppKit
IFLAGS = -I${MLX_DIR}
MLX_LIB = libmlx.dylib
SRCS	= main.c	\
			degree_utils.c	\
			make_ceiling_floor_image.c	\
			print_err.c	\
			check_argv.c	\
			string_utils.c

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
