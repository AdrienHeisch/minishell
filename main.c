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
#include <stdlib.h>
#include <unistd.h>

int	main(int argc, char **argv, char **envp)
{
	t_string	str;
	t_list		*exprs;

	(void)argv;
	if (argc > 1)
		return (MS_USAGE);
	while (1)
	{
		str = ft_string_from(ft_get_next_line(STDIN_FILENO));
		if (!str.content)
			exit(MS_ALLOC);
		if (str.length == 0)
			continue ;
		ft_string_term(&str);
		exprs = parse(&str);
		ft_string_destroy(&str);
		if (!exprs)
			continue ;
		exec(*((t_expr *)exprs->content), envp);
		ft_lstclear(&exprs, (void (*)(void *))free_expr);
	}
	return (0);
}
