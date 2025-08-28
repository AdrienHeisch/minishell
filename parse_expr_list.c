/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expr_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 16:41:30 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/28 16:41:30 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

t_expr	*parse_expr_list(t_list **tokens, t_expr *prev)
{
	t_expr	*expr;
	t_list	*token;

	expr = malloc(sizeof(t_expr));
	if (!expr)
		exit(MS_ALLOC);
	expr->type = EX_LIST;
	token = ft_lstpop_front(tokens);
	expr->data.expr_list.is_or = ((t_token *)token->content)->type == TK_OR;
	ft_lstdelone(token, (void (*)(void *))free_token);
	if (!prev)
		return (free(expr), NULL);
	expr->data.expr_list.left = prev;
	expr->data.expr_list.right = parse(tokens);
	if (!expr->data.expr_list.right)
		return (free(expr), NULL);
	// if (!expr->data.expr_list.right || expr->data.pipe.right->type != EX_CMD
	// 	|| !expr->data.pipe.right->data.cmd.args)
	// 	return (free(expr->data.pipe.right), free(expr), NULL);
	return (expr);
}
