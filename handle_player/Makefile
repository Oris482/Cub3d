CC = cc
NAME = a.out
CFLAGS = #-Wall -Wextra -Werror
IFLAGS = -I mlx
LFLAGS = -L mlx -lmlx -framework OpenGL -framework AppKit
MLX_DIR = ./mlx
MLX_LIB = libmlx.dylib
SRCS	= main.c	\
			init_player.c	\
			move_player.c	\
			rotate_player.c	\
			degree_utils.c

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
