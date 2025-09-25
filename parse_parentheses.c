/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_parentheses.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 00:56:57 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/29 00:56:57 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

/// errno will be set on error
t_expr	*parse_parentheses(t_list **tokens)
{
	t_list	*token;
	t_expr	*expr;

	errno = 0;
	expr = malloc(sizeof(t_expr));
	if (!expr)
		return (NULL);
	expr->type = EX_PARENTHESES;
	expr->fd_in = STDIN_FILENO;
	expr->fd_out = STDOUT_FILENO;
	expr->redirs = NULL;
	token = ft_lstpop_front(tokens);
	while (((t_token *)token->content)->type == TK_REDIR)
	{
		add_redirection(token, &expr->redirs);
		ft_lstdelone(token, (void (*)(void *))free_token);
		token = ft_lstpop_front(tokens);
	}
	ft_lstdelone(token, (void (*)(void *))free_token);
	expr->data.paren.inner = parse(tokens);
	if (errno || !expr->data.paren.inner)
		return (free_expr(expr), NULL);
	token = ft_lstpop_front(tokens);
	if (!token || ((t_token *)token->content)->type != TK_PARCLOSE)
		return (free_expr(expr), NULL);
	ft_lstdelone(token, (void (*)(void *))free_token);
	while (*tokens && ((t_token *)(*tokens)->content)->type == TK_REDIR)
	{
		token = ft_lstpop_front(tokens);
		add_redirection(token, &expr->redirs);
		ft_lstdelone(token, (void (*)(void *))free_token);
	}
	return (expr);
}
