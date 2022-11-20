/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaesjeon <jaesjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:59:53 by jaesjeon          #+#    #+#             */
/*   Updated: 2022/11/12 19:01:53 by jaesjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdlib.h>

void	multi_free(void *ptr1, void *ptr2, void *ptr3, void *ptr4)
{
	free(ptr1);
	free(ptr2);
	free(ptr3);
	free(ptr4);
}

void	*my_malloc(size_t len)
{
	void	*ptr;

	ptr = malloc(len);
	if (ptr == NULL)
		exit_with_err("Malloc error", E_NOMEM);
	return (ptr);
}

void	ft_memset(void *ptr, unsigned char value, size_t size)
{
	size_t			idx;
	unsigned char	*target;

	target = (unsigned char *)ptr;
	idx = 0;
	while (idx < size)
		target[idx++] = value;
}

void	remove_newline(char *line)
{
	while (*line)
	{
		if (*line == '\n')
			*line = '\0';
		line++;
	}
}
