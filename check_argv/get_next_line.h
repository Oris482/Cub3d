/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaesjeon <jaesjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 11:43:27 by jaesjeon          #+#    #+#             */
/*   Updated: 2022/10/08 08:50:02 by jaesjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

typedef struct s_fdlist
{
	char			*cont;
	int				myfd;
	size_t			length;
	int				nlidx;
	struct s_fdlist	*prev;
	struct s_fdlist	*next;
}	t_fdlist;

size_t		gft_strlen(const char *s);
char		*gft_strjoin(char const *s1, char const *s2, ssize_t s2_len);
char		*gft_substr(char const *s, unsigned int start, size_t len);
int			gft_isinnl(char *cont);
t_fdlist	*gft_lstnew(int fd);

char		*get_next_line(int fd);

#endif