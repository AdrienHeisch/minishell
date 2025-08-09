/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:01:13 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/06 13:01:13 by aheisch          ###   ########.fr       */
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
	exprs = NULL;
	while (tokens)
	{
		expr = NULL;
		if (((t_token *)tokens->content)->type == TK_PIPE)
			expr = parse_pipe(&tokens, &exprs);
		else if (((t_token *)tokens->content)->type == TK_ARG)
			expr = parse_cmd(&tokens);
		else
			exit(42);
		ft_lstadd_back(&exprs, ft_lstnew(expr));
	}
	return (exprs);
}
