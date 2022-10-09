/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaesjeon <jaesjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 11:43:17 by jaesjeon          #+#    #+#             */
/*   Updated: 2022/10/08 08:50:24 by jaesjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	gft_strlen(const char *s)
{
	size_t	idx;

	idx = 0;
	if (s == NULL)
		return (0);
	while (*s != '\0')
	{
		idx++;
		s++;
	}
	return (idx);
}

char	*gft_strjoin(char const *s1, char const *s2, ssize_t s2_len)
{
	size_t	s1_len;
	char	*joined;
	size_t	idx;

	s1_len = gft_strlen(s1);
	joined = (char *)malloc(s1_len + s2_len + 1);
	if (joined == NULL)
		return (NULL);
	idx = 0;
	while (s1_len-- > 0)
	{
		joined[idx] = s1[idx];
		idx++;
	}
	if (s1 != NULL)
		free((void *)s1);
	while (s2_len-- > 0)
		joined[idx++] = *s2++;
	joined[idx] = '\0';
	return (joined);
}

char	*gft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	str_len;
	size_t	idx;

	str_len = gft_strlen(s);
	if ((str_len - start) < len && str_len > start)
		len = str_len - start;
	else if (str_len <= start)
		len = 0;
	sub = (char *)malloc(len + 1);
	if (sub == NULL)
		return (NULL);
	if (start >= str_len)
	{
		*sub = '\0';
		return (sub);
	}
	idx = 0;
	while (s[start] != '\0' && len > 0)
	{
		sub[idx++] = s[start++];
		len--;
	}
	sub[idx] = '\0';
	return (sub);
}

int	gft_isinnl(char *cont)
{
	int	idx;

	idx = 0;
	if (cont == NULL)
		return (-1);
	while (cont[idx])
	{
		if (cont[idx] == '\n')
			return (idx);
		idx++;
	}
	return (-1);
}

t_fdlist	*gft_lstnew(int fd)
{
	t_fdlist	*node;

	node = (t_fdlist *)malloc(sizeof(t_fdlist));
	if (node == NULL)
		return (NULL);
	node->myfd = fd;
	node->cont = NULL;
	node->length = 0;
	node->nlidx = -1;
	node->prev = NULL;
	node->next = NULL;
	return (node);
}