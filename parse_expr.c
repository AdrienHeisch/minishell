/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 18:23:11 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/11 18:23:11 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <errno.h>

static bool	is_op(t_token_type token)
{
	return (token == TK_PIPE || token == TK_AND || token == TK_OR);
}

/// errno will be set on error
t_expr	*parse_expr(t_list **tokens, t_expr **prev)
{
	t_token	*token;
	t_expr	*expr;

	errno = 0;
	if (!tokens || !*tokens)
		return (print_error_msg("syntax error: unexpected end of file"), NULL);
	token = (t_token *)(*tokens)->content;
	if (!token)
		return (NULL);
	if (is_op(token->type))
		expr = parse_binop(tokens, prev);
	else if (token->type == TK_ARG || token->type == TK_REDIR)
		expr = parse_cmd(tokens);
	else if (token->type == TK_PAROPEN)
		expr = parse_parentheses(tokens);
	else
		return (NULL);
	if (errno)
		return (free_expr(expr), NULL);
	return (expr);
}
