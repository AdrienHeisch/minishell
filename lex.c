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
		// if (str->content[*idx] == '$')
		// {
		// 	if (del == '\'')
		// 		ft_string_cat(&arg, "\\");
		// 	else if (*idx + 1 < str->length && (str->content[*idx + 1] == '"'
		// 			|| str->content[*idx + 1] == '\''))
		// 	{
		// 		(*idx)++;
		// 		continue ;
		// 	}
		// }
		ft_string_ncat(&arg, &str->content[*idx], 1);
		(*idx)++;
	}
	if (del)
		ft_string_destroy(&arg);
	return (arg);
}

static t_token	*get_token(t_string *str, size_t *idx)
{
	t_token	token;
	t_token	*cell;
	size_t	len;
	char	c;

	len = 0;
	c = str->content[*idx];
	if (is_arg(c))
	{
		token.type = TK_ARG;
		token.data.arg.string = parse_arg(str, idx);
		if (!token.data.arg.string.content)
			token.type = TK_INVALID;
	}
	// else if (c == '\'')
	// {
	// 	(*idx)++;
	// 	while (*idx + len < str->length && str->content[*idx + len] != '\'')
	// 		len++;
	// 	if (!str->content[*idx + len])
	// 		exit(-1);
	// 	token.type = TK_ARG;
	// 	token.data.arg.expand = false;
	// 	token.data.arg.string = ft_string_new();
	// 	ft_string_ncat(&token.data.arg.string, &str->content[*idx], len);
	// 	len++;
	// }
	// else if (c == '"')
	// {
	// 	(*idx)++;
	// 	while (*idx + len < str->length && str->content[*idx + len] != '"')
	// 		len++;
	// 	if (!str->content[*idx + len])
	// 		exit(-1);
	// 	token.type = TK_ARG;
	// 	token.data.arg.expand = true;
	// 	token.data.arg.string = ft_string_new();
	// 	ft_string_ncat(&token.data.arg.string, &str->content[*idx], len);
	// 	len++;
	// }
	else if (c == '|')
	{
		token.type = TK_PIPE;
		len = 1;
	}
	// TODO redir fd
	else if (c == '<')
	{
		token.type = TK_REDIR_IN;
		token.data.redir.fd = 0;
		len = 1;
	}
	else if (c == '>')
	{
		token.type = TK_REDIR_OUT;
		token.data.redir.fd = 1;
		len = 1;
	}
	else
	{
		return (NULL);
	}
	*idx += len;
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
