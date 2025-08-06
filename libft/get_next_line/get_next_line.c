/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 23:06:05 by aheisch           #+#    #+#             */
/*   Updated: 2025/06/04 17:29:45 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef FT_GNL_BUFFER_SIZE
# define FT_GNL_BUFFER_SIZE 64
#endif

int			find_char(char *str, unsigned int n, char c);

static void	ft_gnl_lstadd_back(t_gnl_list **lst, t_gnl_list *el)
{
	t_gnl_list	*last;

	if (!*lst)
	{
		*lst = el;
		return ;
	}
	last = *lst;
	while (last->next)
		last = last->next;
	last->next = el;
}

static void	ft_gnl_lstdel(t_gnl_list **list_start, t_gnl_list *el)
{
	t_gnl_list	*prev;
	t_gnl_list	*list;

	list = *list_start;
	prev = NULL;
	while (list)
	{
		if (list == el)
		{
			if (prev)
				prev->next = el->next;
			else
				*list_start = el->next;
			prev = list;
			list = list->next;
			free(el->str.content);
			free(el);
			continue ;
		}
		prev = list;
		list = list->next;
	}
}

static char	*get_next_line1(t_gnl_list **list_start, t_gnl_list *list, int fd);
static char	*get_next_line2(t_gnl_list **list_start, t_gnl_list *list,
				int nl_idx);

char	*ft_get_next_line(int fd)
{
	static t_gnl_list	*list_start = NULL;
	t_gnl_list			*list;

	if (fd == -1)
		return (ft_gnl_lstdel(&list_start, NULL), NULL);
	list = list_start;
	while (list && list->fd != fd)
		list = list->next;
	if (!list)
	{
		list = (t_gnl_list *)malloc(sizeof(t_gnl_list));
		if (!list)
			return (NULL);
		list->str = ft_string_new();
		if (!list->str.content)
		{
			free(list);
			return (NULL);
		}
		list->fd = fd;
		list->next = NULL;
		ft_gnl_lstadd_back(&list_start, list);
	}
	return (get_next_line1(&list_start, list, fd));
}

static char	*get_next_line1(t_gnl_list **list_start, t_gnl_list *list, int fd)
{
	char	buf[FT_GNL_BUFFER_SIZE];
	int		n_read;
	int		nl_idx;

	nl_idx = -1;
	while (1)
	{
		nl_idx = find_char(list->str.content, list->str.length, '\n');
		if (nl_idx > -1)
			break ;
		n_read = read(fd, buf, FT_GNL_BUFFER_SIZE);
		if (n_read == -1)
		{
			ft_gnl_lstdel(list_start, list);
			return (NULL);
		}
		if (n_read == 0)
			break ;
		if (!ft_string_ncat(&list->str, buf, n_read))
		{
			ft_gnl_lstdel(list_start, list);
			ft_putstr_fd("alloc_error\n", 1);
			return (NULL);
		}
	}
	return (get_next_line2(list_start, list, nl_idx));
}

static char	*get_next_line2(t_gnl_list **list_start, t_gnl_list *list,
		int nl_idx)
{
	char	*out;

	if (list->str.length == 0)
	{
		ft_gnl_lstdel(list_start, list);
		return (NULL);
	}
	if (nl_idx > -1)
	{
		nl_idx++;
		out = ft_strndup(list->str.content, nl_idx);
		ft_strncpy(list->str.content, list->str.content + nl_idx,
			list->str.length - nl_idx);
		list->str.length -= nl_idx;
	}
	else
	{
		out = ft_strndup(list->str.content, list->str.length);
		ft_gnl_lstdel(list_start, list);
	}
	return (out);
}
