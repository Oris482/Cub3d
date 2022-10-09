/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaesjeon <jaesjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 11:42:53 by jaesjeon          #+#    #+#             */
/*   Updated: 2022/10/08 08:49:45 by jaesjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	gft_lstdelone(t_fdlist *lst)
{
	t_fdlist	*prev;
	t_fdlist	*next;

	prev = lst->prev;
	next = lst->next;
	if (lst->cont != NULL)
	{
		free(lst->cont);
		lst->cont = NULL;
	}
	if (next != NULL)
		next->prev = prev;
	if (prev != NULL)
	{
		prev->next = lst->next;
		free(lst);
	}
}

static int	read_line(int fd, t_fdlist *cur)
{
	ssize_t		len;
	char		*buffer;

	buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (buffer == NULL)
		return (-1);
	while (1)
	{
		len = read(fd, buffer, BUFFER_SIZE);
		if (len <= 0)
			break ;
		buffer[len] = '\0';
		cur->cont = gft_strjoin(cur->cont, buffer, len);
		if (cur->cont == NULL)
			return (-1);
		cur->nlidx = gft_isinnl(cur->cont);
		if (cur->nlidx != -1)
			break ;
	}
	free(buffer);
	if (cur->cont == NULL || *(cur->cont) == '\0' || len < 0)
		return (-1);
	cur->length = gft_strlen(cur->cont);
	return (0);
}

static char	*make_line(int fd, t_fdlist *cur)
{
	char	*ret;
	char	*tmp;

	if (read_line(fd, cur) == -1)
	{
		gft_lstdelone(cur);
		return (NULL);
	}
	if (cur->nlidx == -1)
		ret = gft_substr(cur->cont, 0, cur->length);
	else
		ret = gft_substr(cur->cont, 0, cur->nlidx + 1);
	if (ret == NULL)
	{
		gft_lstdelone(cur);
		return (NULL);
	}
	tmp = gft_substr(cur->cont, gft_strlen(ret), cur->length);
	free(cur->cont);
	cur->cont = tmp;
	cur->nlidx = gft_isinnl(cur->cont);
	cur->length = gft_strlen(cur->cont);
	if (tmp == NULL || *tmp == '\0')
		gft_lstdelone(cur);
	return (ret);
}

static char	*makelst(int fd, t_fdlist **head)
{
	t_fdlist	*node;
	t_fdlist	*cur;

	cur = *head;
	while (cur != NULL)
	{
		if (cur->myfd == fd)
			return (make_line(fd, cur));
		if (cur->next == NULL)
			break ;
		cur = cur->next;
	}
	node = gft_lstnew(fd);
	if (node == NULL)
		return (NULL);
	node->prev = cur;
	cur->next = node;
	return (make_line(fd, node));
}

char	*get_next_line(int fd)
{
	static t_fdlist	*head;
	t_fdlist		*tmp;
	char			*ret;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	if (head == NULL)
	{
		head = gft_lstnew(fd);
		if (head == NULL)
			return (NULL);
	}
	ret = makelst(fd, &head);
	if (head->cont == NULL)
	{
		tmp = head;
		if (head->next != NULL)
			head = head->next;
		else
			head = NULL;
		free(tmp);
	}
	return (ret);
}