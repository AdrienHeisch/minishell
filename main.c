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

	(void)argv;
	if (argc > 1)
		return (MS_USAGE);
	data.envp = envp;
	data.status = 0;
	while (1)
	{
		str = ft_string_from(readline("> "));
		if (!str.content)
			return (ft_get_next_line(-1), MS_ALLOC);
		if (str.length == 0)
			continue ;
		exprs = parse(&str);
		if (!exprs)
		{
			ft_string_destroy(&str);
			continue ;
		}
		// ft_lstiter(exprs, (void (*)(void *))print_expr);
		exec(*((t_expr *)exprs->content), &data);
			// TODO multiple expressions
		ft_lstclear(&exprs, (void (*)(void *))free_expr);
		add_history(str.content);
		ft_string_destroy(&str);
	}
	return (ft_get_next_line(-1), ft_string_destroy(&str), MS_SUCCESS);
}
