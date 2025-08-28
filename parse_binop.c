/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_binop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:00:37 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/28 21:18:57 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

static t_operator	get_op(t_token_type token)
{
	if (token == TK_PIPE)
		return (OP_PIPE);
	if (token == TK_AND)
		return (OP_AND);
	if (token == TK_OR)
		return (OP_OR);
	exit(MS_UNREACHABLE);
}

t_expr	*parse_binop(t_list **tokens, t_expr *prev)
{
	t_expr	*expr;
	t_list	*token;

	expr = malloc(sizeof(t_expr));
	if (!expr)
		exit(MS_ALLOC);
	expr->type = EX_BINOP;
	token = ft_lstpop_front(tokens);
	expr->data.binop.op = get_op(((t_token *)token->content)->type);
	ft_lstdelone(token, (void (*)(void *))free_token);
	if (!prev)
		return (free(expr), NULL);
	expr->data.binop.left = prev;
	expr->data.binop.right = parse_expr(tokens, prev);
	if (!expr->data.binop.right || expr->data.binop.right->type != EX_CMD
		|| !expr->data.binop.right->data.cmd.args)
		// TODO test with expr lists on either side
		return (free(expr->data.binop.right), free(expr), NULL);
	return (expr);
}
