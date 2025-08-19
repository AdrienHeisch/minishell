/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:47:53 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/04 15:47:53 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

static char **dup_env(char **envp)
{
	char	**dup;
	size_t	len;
	size_t	idx;
	char	*cwd;

	len = 0;
	while (envp[len])
		len++;
	dup = ft_calloc(len + 1, sizeof(char *));
	idx = 0;
	while (envp[idx])
	{
		dup[idx] = ft_strdup(envp[idx]);
		idx++;
	}
	dup[len] = 0;
	cwd = getcwd(NULL, 0);
	if (cwd)
		ft_setenv(&dup, "PWD", cwd, true);
	return (dup);
}

static void	parse_and_exec(t_string *str, t_shell_data *data)
{
	t_list	*exprs;

	if (is_whitespace(str))
	{
		data->status = 0;
		return ;
	}
	exprs = parse(str);
	if (!exprs)
	{
		data->status = 2;
		return ;
	}
	exec(((t_expr *)exprs->content), data);
	ft_lstclear(&exprs, (void (*)(void *))free_expr);
	add_history(str->content);
}

int	main(int argc, char **argv, char **envp)
{
	t_string		str;
	t_shell_data	data;
	char			**tab;
	size_t			idx;

	data.envp = dup_env(envp);
	data.status = 0;
	if (argc == 3 && ft_strncmp("-c", argv[1], 3) == 0)
	{
		tab = ft_split(argv[2], '\n');
		idx = 0;
		while (tab[idx])
		{
			str = ft_string_new();
			ft_string_cat(&str, tab[idx]);
			parse_and_exec(&str, &data);
			ft_string_destroy(&str);
			idx++;
		}
		return (data.status);
	}
	if (argc > 1)
		return (MS_USAGE);
	str.content = NULL;
	while (1)
	{
		ft_string_destroy(&str);
		str = ft_string_from(readline("> "));
		if (!str.content)
			return (MS_ALLOC);
		parse_and_exec(&str, &data);
	}
	ft_string_destroy(&str);
	return (MS_SUCCESS);
}
