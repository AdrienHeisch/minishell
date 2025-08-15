/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:01:13 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/11 19:59:12 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

t_list	*parse(t_string *str)
{
	t_list	*tokens;
	t_list	*exprs;
	t_expr	*expr;

	tokens = lex(str);
	// ft_lstiter(tokens, (void (*)(void *))print_token);
	exprs = NULL;
	while (tokens)
	{
		expr = parse_expr(&tokens, &exprs);
		if (!expr)
			return (ft_lstclear(&tokens, (void (*)(void *))free_token), ft_lstclear(&exprs, (void (*)(void *))free_expr), NULL);
		ft_lstadd_back(&exprs, ft_lstnew(expr));
	}
	// ft_lstiter(exprs, (void (*)(void *))print_expr);
	return (exprs);
}
