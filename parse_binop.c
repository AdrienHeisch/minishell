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
#include <errno.h>
#include <stdlib.h>

static t_operator	get_op(t_token_type token)
{
	if (token == TK_PIPE)
		return (OP_PIPE);
	if (token == TK_AND)
		return (OP_AND);
	if (token == TK_OR)
		return (OP_OR);
	exit(ERR_UNREACHABLE);
}

static int	get_precedence(t_operator op)
{
	if (op == OP_PIPE)
		return (1);
	return (0);
}

/// errno will be set on error
t_expr	*parse_binop(t_list **tokens, t_expr **prev)
{
	t_expr	*expr;
	t_list	*token;
	t_expr	*old;
	t_expr	*new;
	t_expr	*tmp;

	errno = 0;
	expr = malloc(sizeof(t_expr));
	if (!expr)
		return (NULL);
	expr->type = EX_BINOP;
	expr->redirs = NULL;
	token = ft_lstpop_front(tokens);
	expr->u_data.binop.op = get_op(((t_token *)token->content)->type);
	ft_lstdelone(token, (void (*)(void *))free_token);
	if (!prev || !*prev)
		return (free(expr), NULL);
	expr->u_data.binop.left = *prev;
	*prev = NULL;
	if (expr->u_data.binop.op == OP_PIPE)
	{
		expr->u_data.binop.right = parse_expr(tokens, &expr->u_data.binop.left);
		if (errno)
			return (free_expr(expr), NULL);
		if (!expr->u_data.binop.right)
			return (print_error_msg("unexpected token"), free_expr(expr), NULL);
	}
	else
	{
		expr->u_data.binop.right = parse(tokens);
		if (errno)
			return (free_expr(expr), NULL);
		if (!expr->u_data.binop.right)
			return (print_error_msg("unexpected token"), free_expr(expr), NULL);
		if (expr->u_data.binop.right->type == EX_BINOP
			&& get_precedence(expr->u_data.binop.right->u_data.binop.op) <= get_precedence(expr->u_data.binop.op))
		{
			old = expr;
			new = old->u_data.binop.right;
			tmp = expr->u_data.binop.right->u_data.binop.left;
			old->u_data.binop.right = tmp;
			new->u_data.binop.left = old;
			expr = new;
		}
	}
	return (expr);
}
