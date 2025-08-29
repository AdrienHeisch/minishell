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

static int	get_precedence(t_operator op)
{
	if (op == OP_PIPE)
		return (1);
	return (0);
}

t_expr	*parse_binop(t_list **tokens, t_expr **prev)
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
	if (!prev || !*prev)
		return (free(expr), NULL);
	expr->data.binop.left = *prev;
	*prev = NULL;
	// TODO use operator precedence here (remove this condition)
	if (expr->data.binop.op == OP_PIPE)
	{
		expr->data.binop.right = parse_expr(tokens, &expr->data.binop.left);
		if (!expr->data.binop.right)
			return (free(expr->data.binop.right), free(expr), ft_putstr_fd("unexpected token\n", 2), NULL);
	}
	else
	{
		expr->data.binop.right = parse(tokens);
		if (!expr->data.binop.right)
			return (free(expr), NULL);
		if (expr->data.binop.right->type == EX_BINOP
			&& get_precedence(expr->data.binop.right->data.binop.op) <= get_precedence(expr->data.binop.op))
		{
			t_expr *old = expr;
			t_expr *new = old->data.binop.right;
			t_expr *tmp = expr->data.binop.right->data.binop.left;
			old->data.binop.right = tmp;
			new->data.binop.left = old;
			expr = new;
		}
	}
	return (expr);
}
