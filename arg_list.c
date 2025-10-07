/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 21:21:42 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/11 21:21:42 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static t_err	expand_args(t_cmd cmd, t_shell_data *shell_data,
		t_list **expanded)
{
	t_list	*lst;
	t_list	*arg_list;

	*expanded = NULL;
	arg_list = cmd.args;
	while (arg_list && arg_list->content)
	{
		errno = 0;
		lst = expand_arg(&((t_arg_data *)arg_list->content)->string, shell_data,
				false);
		if (errno)
			return (ft_lstclear(expanded, lstclear_string), ft_lstclear(&lst,
					lstclear_string), ERR_SYSTEM);
		while (lst)
			ft_lstadd_back(expanded, ft_lstpop_front(&lst));
		arg_list = arg_list->next;
	}
	return (ERR_OK);
}

/// Will return null and errno will be set on error
char	**make_arg_list(t_cmd cmd, t_shell_data *shell_data)
{
	char	**args;
	size_t	idx;
	t_list	*arg_list;
	t_list	*expanded;

	if (expand_args(cmd, shell_data, &expanded))
		return (NULL);
	args = ft_calloc((ft_lstsize(expanded) + 1), sizeof(char *));
	if (!args)
		return (ft_lstclear(&expanded, lstclear_string), NULL);
	idx = 0;
	arg_list = expanded;
	while (arg_list && arg_list->content)
	{
		args[idx] = ft_strdup(
				((t_arg_data *)arg_list->content)->string.content);
		if (!args[idx])
			return (ft_lstclear(&expanded, lstclear_string),
				free_tab((void ***)&args), NULL);
		idx++;
		arg_list = arg_list->next;
	}
	ft_lstclear(&expanded, lstclear_string);
	return (args);
}
