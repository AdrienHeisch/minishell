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

t_expr	*parse(t_list **tokens)
{
	t_expr	*expr;
	t_expr	*new;

	expr = NULL;
	while (*tokens)
	{
		if (((t_token *)(*tokens)->content)->type == TK_PARCLOSE)
			break ;
		new = parse_expr(tokens, &expr);
		if (!new)
			return (free_expr(expr), NULL);
		if (expr)
		{
			print_expr(expr);
			print_expr(new);
			exit(MS_LOGIC_ERROR);
		}
		expr = new;
	}
	// print_expr(expr);
	return (expr);
}
