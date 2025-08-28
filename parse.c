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

t_expr	*parse_tokens(t_list **tokens)
{
	t_expr	*expr;
	t_expr	*new;

	expr = NULL;
	while (*tokens)
	{
		new = parse_expr(tokens, expr);
		if (!new)
			return (ft_lstclear(tokens, (void (*)(void *))free_token),
				free_expr(expr), NULL);
		expr = new;
	}
	// print_expr(expr);
	return (expr);
}

t_list	*parse(t_string *str)
{
	t_list	*tokens;
	t_expr	*expr;

	tokens = lex(str);
	// ft_lstiter(tokens, (void (*)(void *))print_token);
	expr = parse_tokens(&tokens);
	if (expr)
		return (ft_lstnew(expr));
	else
		return (NULL);
}
