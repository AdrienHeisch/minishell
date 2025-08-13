/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 12:56:29 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/06 12:56:29 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

static bool	is_arg(char c)
{
	static const char	list[] = {' ', '\t', '\n', '|', '<', '>', 0};
	size_t				i;

	i = 0;
	while (list[i])
	{
		if (list[i] == c)
			return (false);
		i++;
	}
	return (true);
}

static t_string	parse_arg(t_string *str, size_t *idx)
{
	t_string	arg;
	char		del;

	arg = ft_string_new();
	del = '\0';
	while (*idx < str->length)
	{
		if (!del)
		{
			if (!is_arg(str->content[*idx]))
				break ;
			if (str->content[*idx] == '\'' || str->content[*idx] == '"')
				del = str->content[*idx];
		}
		else if (del == str->content[*idx])
			del = '\0';
		ft_string_ncat(&arg, &str->content[*idx], 1);
		(*idx)++;
	}
	// STRICTLY NOT INTERPRETING UNCLOSED QUOTES RATHER THAN RAISING AN ERROR
	// if (del)
	// 	ft_string_destroy(&arg);
	return (arg);
}

static t_token	*get_token(t_string *str, size_t *idx)
{
	t_token	token;
	t_token	*cell;
	char	c;

	c = str->content[*idx];
	if (is_arg(c))
	{
		token.type = TK_ARG;
		token.data.arg.string = parse_arg(str, idx);
		if (!token.data.arg.string.content)
			token.type = TK_INVALID;
	}
	else if (c == '|')
	{
		token.type = TK_PIPE;
		(*idx)++;
	}
	// TODO redir fd
	else if (c == '<')
	{
		token.type = TK_REDIR_IN;
		token.data.redir.fd = 0;
		(*idx)++;
	}
	else if (c == '>')
	{
		token.type = TK_REDIR_OUT;
		token.data.redir.fd = 1;
		(*idx)++;
	}
	else
		return (NULL);
	cell = malloc(sizeof(t_token));
	*cell = token;
	return (cell);
}

t_list	*lex(t_string *str)
{
	t_list	*tokens;
	t_list	*new_list;
	t_token	*token;
	size_t	idx;

	tokens = NULL;
	idx = 0;
	while (idx < str->length)
	{
		if (str->content[idx] == ' ')
		{
			idx++;
			continue ;
		}
		token = get_token(str, &idx);
		if (!token)
			break ;
		if (token->type == TK_INVALID)
			return (ft_lstclear(&tokens, (void (*)(void *))free_token), NULL);
		new_list = ft_lstnew(token);
		if (!new_list)
			break ; // TODO ????
		ft_lstadd_back(&tokens, new_list);
	}
	return (tokens);
}
