/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galauren <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 13:07:38 by galauren          #+#    #+#             */
/*   Updated: 2025/09/12 17:07:03 by galauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

/// errno will be set on error
static bool	is_correct_pattern(char *name, char *pattern)
{
	int		n_idx;
	int		p_idx;
	int		len;
	char	*target;
	char	*found;

	p_idx = 0;
	n_idx = 0;
	while (pattern[p_idx])
	{
		while (name[n_idx] == pattern[p_idx])
		{
			++n_idx;
			++p_idx;
			if (!name[n_idx] && !pattern[p_idx])
				return (true);
		}
		if (name[n_idx] != pattern[p_idx] && pattern[p_idx] != '*')
			return (false);
		if (pattern[p_idx] == '*')
		{
			len = 0;
			while (pattern[p_idx + 1 + len] && pattern[p_idx + 1 + len] != '*')
				len++;
			target = ft_substr(pattern, p_idx + 1, len);
			if (!target)
				return (false);
			if (*target)
			{
				found = ft_strnstr(name + n_idx, target, ft_strlen(name) + 1);
				free(target);
				if (!found)
					return (false);
				n_idx = found - name + len;
			}
			else
				return (free(target), true);
			p_idx += len;
		}
		++p_idx;
	}
	return (true);
}

static void	sort_list(t_list **list)
{
	t_list	*lst;
	t_list	*prev;
	t_list	*swap;

	lst = *list;
	prev = NULL;
	while (lst && lst->content && lst->next && lst->next->content)
	{
		if (ft_strncmp(((t_string *)lst->content)->content,
				((t_string *)lst->next->content)->content,
				((t_string *)lst->content)->length) > 0)
		{
			if (prev)
				prev->next = lst->next;
			else
				*list = lst->next;
			swap = lst->next->next;
			lst->next->next = lst;
			lst->next = swap;
			lst = NULL;
		}
		prev = lst;
		if (!lst)
			lst = *list;
		else
			lst = lst->next;
	}
}

/// Will return null and errno will be set on error
t_list	*expand_wildcards(char *pattern)
{
	DIR				*dir;
	struct dirent	*ent;
	t_list			*ret;
	t_string		name;
	char			*cwd;

	ret = NULL;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (print_error_prefix("error retrieving current directory: getcwd"),
			NULL);
	dir = opendir(cwd);
	free(cwd);
	if (dir == NULL)
		return (print_error(), NULL);
	while (1)
	{
		ent = readdir(dir);
		if (!ent)
			break ;
		if (!ft_strncmp(ent->d_name, ".", 1))
			continue ;
		if (is_correct_pattern(ent->d_name, pattern))
		{
			name = ft_string_new();
			if (!name.content)
				return (closedir(dir), ft_lstclear(&ret, free), NULL);
			if (!ft_string_cat(&name, ent->d_name))
				return (closedir(dir), ft_lstclear(&ret, free), NULL);
			if (lstadd_back_string(&ret, name))
				return (closedir(dir), ft_lstclear(&ret, free), NULL);
		}
		else if (errno)
			return (closedir(dir), ft_lstclear(&ret, free), NULL);
	}
	if (closedir(dir))
		return (ft_lstclear(&ret, free), NULL);
	sort_list(&ret);
	return (ret);
}
