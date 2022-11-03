/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaesjeon <jaesjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:57:47 by jaesjeon          #+#    #+#             */
/*   Updated: 2022/11/03 11:57:54 by jaesjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STRING_H
# define FT_STRING_H
# include <stdlib.h>
# include "cub3d.h"
# include "get_next_line.h"

// get_next_line.c
char	*get_next_line(int fd);

// string_utils.c
size_t	ft_strlen(const char *s);
char	*ft_strchr(const char *s, const char c);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_substr(const char *start, const char *end);
char	*ft_strcpy(const char *s);

#endif
