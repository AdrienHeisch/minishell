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
#include <unistd.h>

int	main(int argc, char **argv, char **envp)
{
	t_string	str;
	t_list		*exprs;
	t_list		*history;

	(void)argv;
	if (argc > 1)
		return (MS_USAGE);
	history = NULL;
	while (1)
	{
		ft_putstr_fd("> ", 1);
		str = ft_string_from(ft_get_next_line(STDIN_FILENO));
		if (!str.content)
			return (ft_get_next_line(-1), ft_lstclear(&history,
					lstclear_string), MS_ALLOC);
		if (str.length == 0)
			continue ;
		exprs = parse(&str);
		ft_string_destroy(&str);
		if (!exprs)
			continue ;
		exec(*((t_expr *)exprs->content), envp); // TODO multiple expressions
		ft_lstclear(&exprs, (void (*)(void *))free_expr);
	}
	return (ft_get_next_line(-1), ft_string_destroy(&str), ft_lstclear(&history, lstclear_string),
		MS_SUCCESS);
}
