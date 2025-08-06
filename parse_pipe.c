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

t_expr	*parse_pipe(t_list **tokens, t_list **exprs)
{
	t_expr	*expr;
	t_list	*token;

	expr = malloc(sizeof(t_expr));
	if (!expr)
		exit(MS_ALLOC);
	expr->type = EX_PIPE;
	token = ft_lstpop_front(tokens);
	ft_lstdelone(token, (void (*)(void *))free_token);
	expr->data.pipe.left = ft_lstpop_back(exprs)->content;
	expr->data.pipe.right = parse_cmd(tokens);
	return (expr);
}
