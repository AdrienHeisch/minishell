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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

int	is_correct_pattern(char *name, char *pattern)
{
	int		n_idx;
	int		p_idx;
	char	target_char;

	p_idx = 0;
		n_idx = 0;
	target_char = '\0';
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
			int len = 0;
			while (pattern[p_idx + 1 + len] && pattern[p_idx + 1 + len] != '*')
				len++;
			char *target = ft_substr(pattern, p_idx + 1, len);
			if (!target)
				exit(42);
			if (*target)
			{
				char *found = ft_strnstr(name + n_idx, target, ft_strlen(name) + 1);
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

t_string	get_dir_content(char *pattern)
{
	DIR				*dir;
	struct dirent	*ent;
	t_string		ret;

	ret = ft_string_new();
	dir = opendir(getcwd(NULL, 0));
	if (dir != NULL)
	{
		while (1)
		{
			ent = readdir(dir);
			if (!ent)
				break ;
			if (is_correct_pattern(ent->d_name, pattern))
			{
				ft_string_cat(&ret, ent->d_name);
				ft_string_cat(&ret, " ");
			}
		}
		closedir(dir);
	}
	else
	{
 		perror ("");
		ft_string_destroy(&ret);
  		return (ret);
	}
	return (ret);
}

t_string	expand_wildcards(char *expr)
{
	int			i;
	char		*copy;
	t_string	dir;

	i = -1;
	copy = NULL;
	dir = get_dir_content(expr);
	if (!dir.content)
	{
		printf("ForkYou: Error: wildcards's expansion failed !\n");
		return (dir);
	}
	return (dir);
}

int main(int ac, char **av)
{
	t_string	tmp;

	(void)ac;
	tmp = expand_wildcards(av[1]);
	printf("str: |%s|       expanded: |%s|\n", av[1], tmp.content);
	return (0);
}
