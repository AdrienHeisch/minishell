/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 19:26:07 by aheisch           #+#    #+#             */
/*   Updated: 2025/03/10 19:26:07 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstpop_front(t_list **lst)
{
	t_list	*pop;

	if (!lst)
		return (NULL);
	pop = *lst;
	if (!pop)
		return (NULL);
	*lst = pop->next;
	pop->next = NULL;
	return (pop);
}

t_list	*ft_lstpop_back(t_list **lst)
{
	t_list	*el;
	t_list	*pop;

	if (!lst)
		return (NULL);
	el = *lst;
	if (!el)
		return (NULL);
	while (el->next && el->next->next)
		el = el->next;
	if (!el->next)
	{
		pop = el;
		*lst = NULL;
	}
	else
	{
		pop = el->next;
		el->next = NULL;
	}
	return (pop);
}

t_list	*ft_lstget_nth(t_list *lst, size_t n)
{
	while (n > 0)
	{
		if (!lst)
			return (NULL);
		lst = lst->next;
		n--;
	}
	return (lst);
}

static void	shallow_free(void *ptr)
{
	(void)ptr;
}

t_list	*ft_lstcopy_shallow(t_list *src)
{
	t_list	*o_lst;
	t_list	*lst;
	t_list	*prev;

	o_lst = NULL;
	prev = NULL;
	while (src)
	{
		lst = ft_lstnew(src->content);
		if (!lst)
			return (ft_lstclear(&o_lst, shallow_free), NULL);
		if (prev)
			prev->next = lst;
		if (!o_lst)
			o_lst = lst;
		prev = lst;
		src = src->next;
	}
	return (o_lst);
}
