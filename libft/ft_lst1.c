/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:02:30 by aheisch           #+#    #+#             */
/*   Updated: 2025/03/10 18:18:50 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	del(lst->content);
	lst->content = NULL;
	free(lst);
}

void	ft_lstremove(t_list **lst, void *el, void (*del)(void *))
{
	t_list	*prev;
	t_list	*next;

	prev = NULL;
	while (*lst)
	{
		next = (*lst)->next;
		if ((*lst)->content == el)
		{
			if (prev)
				prev->next = next;
			ft_lstdelone(*lst, del);
			if (!prev)
				*lst = NULL;
			return ;
		}
		prev = *lst;
		*lst = next;
	}
	*lst = NULL;
}

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*next;

	while (*lst)
	{
		next = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = next;
	}
	*lst = NULL;
}

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*map_start;
	t_list	*map;
	void	*new_content;

	map_start = NULL;
	while (lst)
	{
		new_content = f(lst->content);
		map = ft_lstnew(new_content);
		if (!map)
		{
			del(new_content);
			ft_lstclear(&map_start, del);
			return (NULL);
		}
		ft_lstadd_back(&map_start, map);
		lst = lst->next;
	}
	return (map_start);
}
