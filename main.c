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
#include <unistd.h>

int	main(int argc, char **argv, char **envp)
{
	t_string		str;
	t_list			*exprs;
	t_shell_data	data;

	data.envp = envp;
	data.status = 0;
	if (argc == 3 && ft_strncmp("-c", argv[1], 2) == 0)
	{
		str = ft_string_new();
		ft_string_cat(&str, argv[2]);
		if (is_whitespace(&str))
			return (ft_string_destroy(&str), 0);
		exprs = parse(&str);
		if (!exprs)
			return (ft_string_destroy(&str), 2);
		exec(((t_expr *)exprs->content), &data);
		ft_lstclear(&exprs, (void (*)(void *))free_expr);
		ft_string_destroy(&str);
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
		if (is_whitespace(&str))
		{
			data.status = 0;
			continue ;
		}
		exprs = parse(&str);
		if (!exprs)
		{
			data.status = 2;
			continue ;
		}
		// TODO multiple expressions
		exec(((t_expr *)exprs->content), &data);
		ft_lstclear(&exprs, (void (*)(void *))free_expr);
		add_history(str.content);
	}
	ft_string_destroy(&str);
	return (MS_SUCCESS);
}
