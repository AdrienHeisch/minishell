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

static bool	is_op(t_token_type token)
{
	return (token == TK_PIPE || token == TK_AND || token == TK_OR);
}

t_expr	*parse_expr(t_list **tokens, t_expr **prev)
{
	t_token	*token;

	if (!tokens || !*tokens)
		return (ft_putstr_fd("minishell: syntax error: unexpected end of file\n",
				2), NULL);
	token = (t_token *)(*tokens)->content;
	if (!token)
		return (NULL);
	if (is_op(token->type))
		return (parse_binop(tokens, prev));
	else if (token->type == TK_ARG || token->type == TK_REDIR)
		return (parse_cmd(tokens));
	else if (token->type == TK_PAROPEN)
		return (parse_parentheses(tokens));
	else
		return (NULL);
}
