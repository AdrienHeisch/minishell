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

static bool	is_arg(t_token_type type)
{
	return (type == TK_ARG);
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
	expr->data.cmd.fd_in = STDIN_FILENO;
	expr->data.cmd.fd_out = STDOUT_FILENO;
	ft_string_destroy(&expr->data.cmd.file_in);
	ft_string_destroy(&expr->data.cmd.file_out);
	while (*tokens && is_arg(((t_token *)(*tokens)->content)->type))
	{
		token = ft_lstpop_front(tokens);
		// TODO build list of redirections
		// if (( (t_token *)token->content )->type == TK_REDIR_IN)
		// {
		// 	// expr->data.cmd.file_in
		// 	continue ;
		// }
		arg_data = malloc(sizeof(t_arg_data));
		if (!arg_data)
			exit(MS_ALLOC);
		arg_data->expand = ((t_token *)token->content)->data.arg.expand;
		arg_data->is_dq = ((t_token *)token->content)->data.arg.is_dq;
		ft_string_move(&((t_token *)token->content)->data.arg.string, &arg_data->string);
		arg = ft_lstnew(arg_data);
		ft_lstadd_back(&expr->data.cmd.args, arg);
		ft_lstdelone(token, (void (*)(void *))free_token);
	}
	return (expr);
}
