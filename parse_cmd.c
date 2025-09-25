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
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

static bool	is_cmd(t_token_type type)
{
	return (type == TK_ARG || type == TK_REDIR);
}

t_err	add_redirection(t_list *token, t_list **list)
{
	t_redir_data	*redir;
	t_list			*redir_list;

	redir = ft_calloc(sizeof(t_redir_data), 1);
	if (!redir)
		return (ERR_SYSTEM);
	redir->fd = ((t_token *)token->content)->u_data.redir.fd;
	redir->type = ((t_token *)token->content)->u_data.redir.type;
	ft_string_move(&((t_token *)token->content)->u_data.redir.file_name,
		&redir->file_name);
	redir_list = ft_lstnew(redir);
	if (!redir_list)
		return (ERR_SYSTEM);
	ft_lstadd_back(list, redir_list);
	return (ERR_OK);
}

/// errno will be set on error
t_expr	*parse_cmd(t_list **tokens)
{
	t_expr		*expr;
	t_list		*token;
	t_arg_data	*arg_data;
	t_list		*arg;

	errno = 0;
	expr = malloc(sizeof(t_expr));
	if (!expr)
		return (NULL);
	expr->type = EX_CMD;
	expr->fd_in = STDIN_FILENO;
	expr->fd_out = STDOUT_FILENO;
	expr->redirs = NULL;
	expr->u_data.cmd.args = NULL;
	while (*tokens && is_cmd(((t_token *)(*tokens)->content)->type))
	{
		token = ft_lstpop_front(tokens);
		if (((t_token *)token->content)->type == TK_REDIR)
		{
			add_redirection(token, &expr->redirs);
			ft_lstdelone(token, (void (*)(void *))free_token);
			continue ;
		}
		arg_data = malloc(sizeof(t_arg_data));
		if (!arg_data)
			return (ft_lstdelone(token, (void (*)(void *))free_token),
				free_expr(expr), NULL);
		ft_string_move(&((t_token *)token->content)->u_data.arg.string,
			&arg_data->string);
		arg = ft_lstnew(arg_data);
		if (!arg)
			return (ft_lstdelone(token, (void (*)(void *))free_token),
				free_expr(expr), free(arg_data), NULL);
		ft_lstadd_back(&expr->u_data.cmd.args, arg);
		ft_lstdelone(token, (void (*)(void *))free_token);
	}
	if (*tokens && ((t_token *)(*tokens)->content)->type == TK_PAROPEN)
		return (free_expr(expr), NULL);
	return (expr);
}
