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
	t_expr		*expr;
	t_list		*token;
	t_arg_data	*arg_data;
	t_list		*arg;

	expr = malloc(sizeof(t_expr));
	if (!expr)
		exit(MS_ALLOC);
	expr->type = EX_CMD;
	expr->data.cmd.args = NULL;
	expr->data.cmd.fd_in = -1;
	expr->data.cmd.fd_out = -1;
	expr->data.cmd.file_in.content = NULL;
	ft_string_destroy(&expr->data.cmd.file_in);
	expr->data.cmd.file_out.content = NULL;
	ft_string_destroy(&expr->data.cmd.file_out);
	expr->data.cmd.output_mode = OUTM_UNSET;
	while (*tokens && is_cmd(((t_token *)(*tokens)->content)->type))
	{
		token = ft_lstpop_front(tokens);
		if (((t_token *)token->content)->type == TK_REDIR)
		{
			if (((t_token *)token->content)->data.redir.type == REDIR_IN)
				ft_string_move(&((t_token *)token->content)->data.redir.file_name,
					&expr->data.cmd.file_in);
			else if (((t_token *)token->content)->data.redir.type == REDIR_OUT)
			{
				ft_string_move(&((t_token *)token->content)->data.redir.file_name,
					&expr->data.cmd.file_out);
				expr->data.cmd.output_mode = OUTM_WRITE;
			}
			else if (((t_token *)token->content)->data.redir.type == REDIR_APPEND)
			{
				if (expr->data.cmd.output_mode != OUTM_WRITE)
				{
					ft_string_move(&((t_token *)token->content)->data.redir.file_name,
						&expr->data.cmd.file_out);
					expr->data.cmd.output_mode = OUTM_APPEND;
				}
			}
			else
				exit(MS_UNREACHABLE);
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
	if (expr->data.cmd.output_mode == OUTM_UNSET)
		expr->data.cmd.output_mode = OUTM_WRITE;
	return (expr);
}
