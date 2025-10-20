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

static int	check_pattern(char *name,
	char *pattern, int *p_idx, int *n_idx)
{
	int		len;
	char	*target;
	char	*found;

	if (pattern[*p_idx] == '*')
	{
		len = 0;
		while (pattern[*p_idx + 1 + len] && pattern[*p_idx + 1 + len] != '*')
			len++;
		target = ft_substr(pattern, *p_idx + 1, len);
		if (!target)
			return (-1);
		if (!*target && !(name[*n_idx]))
			return (free(target), 1);
		else
		{
			found = ft_strnstr(name + *n_idx, target, ft_strlen(name) + 1);
			free(target);
			if (!found)
				return (-1);
			*n_idx = found - name + len;
		}
		*p_idx += len;
	}
	return (0);
}

/// errno will be set on error
static bool	is_correct_pattern(char *name, char *pattern)
{
	int		n_idx;
	int		p_idx;
	t_err	err;

	p_idx = -1;
	n_idx = 0;
	while (pattern[++p_idx])
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
		err = check_pattern(
				name, pattern, &p_idx, &n_idx);
		if (err == 1)
			return (true);
		else if (err == -1)
			return (false);
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
			list_rewire(&swap, &lst);
		}
		prev = lst;
		if (!lst)
			lst = *list;
		else
			lst = lst->next;
	}
}

static int	wildcards_loop(DIR *dir, t_list **ret, char *pattern)
{
	struct dirent	*ent;
	t_string		name;

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
				return (closedir(dir), ft_lstclear(ret, free), 1);
			if (!ft_string_cat(&name, ent->d_name))
				return (closedir(dir), ft_lstclear(ret, free), 1);
			if (lstadd_back_string(ret, name))
				return (closedir(dir), ft_lstclear(ret, free), 1);
		}
		else if (errno)
			return (closedir(dir), ft_lstclear(ret, free), 1);
	}
	return (0);
}

/// Will return null and errno will be set on error
t_list	*expand_wildcards(char *pattern)
{
	DIR				*dir;
	t_list			*ret;
	char			*cwd;
	int				err;

	ret = NULL;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (print_error_prefix("error retrieving current\
 directory: getcwd"), NULL);
	dir = opendir(cwd);
	free(cwd);
	if (dir == NULL)
		return (print_error(), NULL);
	err = wildcards_loop(dir, &ret, pattern);
	if (err)
		return (closedir(dir), NULL);
	if (closedir(dir))
		return (ft_lstclear(&ret, free), NULL);
	return (sort_list(&ret), ret);
}
