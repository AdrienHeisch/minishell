/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 23:57:32 by aheisch           #+#    #+#             */
/*   Updated: 2025/10/08 23:57:32 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

void	list_rewire(t_list **swap, t_list **lst)
{
	*swap = (*lst)->next->next;
	(*lst)->next->next = *lst;
	(*lst)->next = *swap;
	*lst = NULL;
}

/// Returns ERR_OK or ERR_SYSTEM
t_err	lstadd_back_string(t_list **list, t_string str)
{
	t_string	*cell;
	t_list		*new;

	cell = (t_string *)malloc(sizeof(t_string));
	if (!cell)
		return (ERR_SYSTEM);
	*cell = str;
	new = ft_lstnew(cell);
	if (!new)
		return (ERR_SYSTEM);
	ft_lstadd_back(list, new);
	return (ERR_OK);
}

void	free_tab(void ***tab)
{
	size_t	idx;

	if (!*tab)
		return ;
	idx = 0;
	while ((*tab)[idx])
		free((*tab)[idx++]);
	free(*tab);
	*tab = NULL;
}

void	lstclear_string(void *str)
{
	ft_string_delete((t_string **)&str);
}
