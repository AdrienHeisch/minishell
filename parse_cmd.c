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

void	add_redirection(t_list *token, t_list **list)
{
	t_redir_data	*redir;
	t_list			*redir_list;

	redir = ft_calloc(sizeof(t_redir_data), 1);
	if (!redir)
		exit(MS_ALLOC);
	redir->fd = ((t_token *)token->content)->data.redir.fd;
	redir->type = ((t_token *)token->content)->data.redir.type;
	ft_string_move(&((t_token *)token->content)->data.redir.file_name,
		&redir->file_name);
	redir_list = ft_lstnew(redir);
	if (!redir_list)
		exit(MS_ALLOC);
	ft_lstadd_back(list, redir_list);
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
	expr->fd_in = STDIN_FILENO;
	expr->fd_out = STDOUT_FILENO;
	expr->redirs = NULL;
	expr->data.cmd.args = NULL;
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
			exit(MS_ALLOC);
		ft_string_move(&((t_token *)token->content)->data.arg.string,
			&arg_data->string);
		arg = ft_lstnew(arg_data);
		ft_lstadd_back(&expr->data.cmd.args, arg);
		ft_lstdelone(token, (void (*)(void *))free_token);
	}
	if (*tokens && ((t_token *)(*tokens)->content)->type == TK_PAROPEN)
		return (free_expr(expr), NULL);
	return (expr);
}
