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

int	main(int argc, char **argv, char **envp)
{
	t_string	str;
	t_list		*exprs;

	(void)argc;
	str = ft_string_new();
	ft_string_cat(&str, argv[1]);
	exprs = parse(&str);
	// ft_lstiter(exprs, (void (*)(void *))print_expr);
	exec(*((t_expr *)exprs->content), envp);
	ft_string_destroy(&str);
	return (0);
}
