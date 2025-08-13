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

static char **dup_env(char **envp)
{
	char	**dup;
	size_t	len;

	len = 0;
	while (envp[len])
		len++;
	dup = ft_calloc(len + 1, sizeof(char *));
	ft_memcpy(dup, envp, len * sizeof(char *));
	dup[len] = 0;
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
	ft_string_term(str);
	add_history(str->content);
}

int	main(int argc, char **argv, char **envp)
{
	t_string		str;
	t_shell_data	data;

	data.pwd = getenv("PWD");
	data.envp = dup_env(envp);
	data.status = 0;
	if (argc == 3 && ft_strncmp("-c", argv[1], 3) == 0)
	{
		str = ft_string_new();
		ft_string_cat(&str, argv[2]);
		parse_and_exec(&str, &data);
		return (ft_string_destroy(&str), data.status);
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
