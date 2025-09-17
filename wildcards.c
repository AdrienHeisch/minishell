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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int	is_correct_pattern(char *name, char *pattern)
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
				return (1);
		}
		if (name[n_idx] != pattern[p_idx] && pattern[p_idx] != '*')
			return (0);
		if (pattern[p_idx] == '*')
		{
			len = 0;
			while (pattern[p_idx + 1 + len] && pattern[p_idx + 1 + len] != '*')
				len++;
			target = ft_substr(pattern, p_idx + 1, len);
			if (!target)
				exit(42);
			if (*target)
			{
				found = ft_strnstr(name + n_idx, target, ft_strlen(name) + 1);
				free(target);
				if (!found)
					return (0);
				n_idx = found - name + len;
			}
			else
				return (free(target), 1);
			p_idx += len;
		}
		++p_idx;
	}
	return (1);
}

t_list	*expand_wildcards(char *pattern)
{
	DIR				*dir;
	struct dirent	*ent;
	t_list			*ret;
	t_string		name;

	ret = NULL;
	dir = opendir(getcwd(NULL, 0));
	if (dir != NULL)
	{
		while (1)
		{
			ent = readdir(dir);
			if (!ent)
				break ;
			if (!ft_strncmp(ent->d_name, ".", 2) || !ft_strncmp(ent->d_name,
					"..", 3))
				continue ;
			if (is_correct_pattern(ent->d_name, pattern))
			{
				name = ft_string_new();
				ft_string_cat(&name, ent->d_name);
				lstadd_back_string(&ret, name);
			}
		}
		closedir(dir);
	}
	else
	{
		perror("");
		ft_lstclear(&ret, free);
		return (ret);
	}
	return (ret);
}
