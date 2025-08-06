/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:00:03 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/06 13:00:03 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

t_expr	*parse_cmd(t_list **tokens)
{
	t_expr		*expr;
	t_list		*token;
	t_string	*string;
	t_list		*arg;

	expr = malloc(sizeof(t_expr));
	if (!expr)
		exit(MS_ALLOC);
	expr->type = EX_CMD;
	while (*tokens && ((t_token *)(*tokens)->content)->type == TK_ARG)
	{
		token = ft_lstpop_front(tokens);
		string = malloc(sizeof(t_string));
		if (!string)
			exit(MS_ALLOC);
		ft_string_move(&((t_token *)token->content)->data.arg.string, string);
		arg = ft_lstnew(string);
		ft_lstadd_back(&expr->data.cmd.args, arg);
		ft_lstdelone(token, (void (*)(void *))free_token);
	}
	return (expr);
}
