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

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

static bool	is_cmd(t_token_type type)
{
	return (type == TK_ARG || type == TK_REDIR);
}

t_expr	*parse_cmd(t_list **tokens)
{
	t_expr			*expr;
	t_list			*token;
	t_arg_data		*arg_data;
	t_list			*arg;
	t_redir_data	*redir;
	t_list			*redir_list;

	expr = malloc(sizeof(t_expr));
	if (!expr)
		exit(MS_ALLOC);
	expr->type = EX_CMD;
	expr->data.cmd.args = NULL;
	expr->data.cmd.fd_in = 0;
	expr->data.cmd.fd_out = 1;
	expr->data.cmd.redirs = NULL;
	while (*tokens && is_cmd(((t_token *)(*tokens)->content)->type))
	{
		token = ft_lstpop_front(tokens);
		if (((t_token *)token->content)->type == TK_REDIR)
		{
			redir = ft_calloc(sizeof(t_redir_data), 1);
			if (!redir)
				exit(MS_ALLOC);
			*redir = ((t_token *)token->content)->data.redir;
			redir_list = ft_lstnew(redir);
			if (!redir_list)
				exit(MS_ALLOC);
			ft_lstadd_back(&expr->data.cmd.redirs, redir_list);
			ft_lstdelone(token, (void (*)(void *))free_token);
			continue ;
		}
		arg_data = malloc(sizeof(t_arg_data));
		if (!arg_data)
			exit(MS_ALLOC);
		ft_string_move(&((t_token *)token->content)->data.arg.string,
			&arg_data->string);
		arg = ft_lstnew(arg_data);
		ft_lstadd_back(&expr->data.cmd.args, arg);
		ft_lstdelone(token, (void (*)(void *))free_token);
	}
	return (expr);
}
