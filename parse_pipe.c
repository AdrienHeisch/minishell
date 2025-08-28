/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:00:37 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/06 13:00:37 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

t_expr	*parse_pipe(t_list **tokens, t_expr *prev)
{
	t_expr	*expr;
	t_list	*token;

	expr = malloc(sizeof(t_expr));
	if (!expr)
		exit(MS_ALLOC);
	expr->type = EX_PIPE;
	token = ft_lstpop_front(tokens);
	ft_lstdelone(token, (void (*)(void *))free_token);
	if (!prev)
		return (free(expr), NULL);
	expr->data.pipe.left = prev;
	expr->data.pipe.right = parse_expr(tokens, prev);
	if (!expr->data.pipe.right || expr->data.pipe.right->type != EX_CMD
		|| !expr->data.pipe.right->data.cmd.args)
		// TODO test with expr lists on either side
		return (free(expr->data.pipe.right), free(expr), NULL);
	return (expr);
}
