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

#include "minishell.h"

t_expr	*parse_expr(t_list **tokens, t_expr *prev)
{
	t_token	*token;

	if (!tokens || !*tokens)
		return (NULL);
	token = ((t_token *)(*tokens)->content);
	if (!token)
		return (NULL);
	if (token->type == TK_PIPE)
		return (parse_pipe(tokens, prev));
	else if (token->type == TK_AND || token->type == TK_OR)
		return (parse_expr_list(tokens, prev));
	else if (token->type == TK_ARG || token->type == TK_REDIR)
		return (parse_cmd(tokens));
	else
		return (NULL);
}
