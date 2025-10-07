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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static bool	is_arg(char c)
{
	static const char	list[] = {' ', '\t', '\n', '|', '&', '<', '>', ';', '(',
		')', 0};
	size_t				i;

	if (!ft_isprint(c))
		return (false);
	i = 0;
	while (list[i])
	{
		if (list[i] == c)
			return (false);
		i++;
	}
	return (true);
}

t_err	parse_loop(t_string *str, size_t *idx, t_string *arg, bool *can_be_null)
{
	char	del;

	del = '\0';
	while (*idx < str->length)
	{
		if (!del)
		{
			if (!is_arg(str->content[*idx]))
				break ;
			if (str->content[*idx] == '\'' || str->content[*idx] == '"')
			{
				del = str->content[*idx];
				*can_be_null = false;
			}
		}
		else if (del == str->content[*idx])
			del = '\0';
		if (!ft_string_ncat(arg, &str->content[*idx], 1))
			return (ft_string_destroy(arg), ERR_SYSTEM);
		(*idx)++;
	}
	return (ERR_OK);
}

/// Returns ERR_OK or ERR_SYSTEM
static t_err	parse_arg(t_string *str, size_t *idx, t_string *arg)
{
	bool	can_be_null;
	int		ret;

	errno = 0;
	*arg = ft_string_new();
	if (!arg->content)
		return (ERR_SYSTEM);
	can_be_null = true;
	ret = parse_loop(str, idx, arg, &can_be_null);
	if (ret != ERR_OK)
		return (ret);
	if (arg->length == 0 && can_be_null)
		ft_string_destroy(arg);
	return (ERR_OK);
}

t_err	is_pipe_or_arg(t_token *tk, char c, size_t *idx, t_string *str)
{
	if (is_arg(c))
	{
		tk->type = TK_ARG;
		if (parse_arg(str, idx, &tk->u_data.arg.string))
			return (ERR_SYSTEM);
	}
	else if (c == '|')
	{
		tk->type = TK_PIPE;
		(*idx)++;
		if (str->content[*idx] == '|')
		{
			tk->type = TK_OR;
			(*idx)++;
		}
	}
	else
		return (111999);
	return (0);
}

t_err	is_redir_in(t_token *tk, char c, size_t *idx, t_string *str)
{
	if (c == '<')
	{
		tk->type = TK_REDIR;
		tk->u_data.redir.type = REDIR_IN;
		tk->u_data.redir.fd = STDIN_FILENO;
		(*idx)++;
		if (str->content[*idx] == '<')
		{
			tk->u_data.redir.type = REDIR_HEREDOC;
			(*idx)++;
		}
		while (is_whitespace(str->content[*idx]))
			(*idx)++;
		if (!is_arg(str->content[*idx]))
			return (ERR_SYNTAX_ERROR);
		if (parse_arg(str, idx, &tk->u_data.redir.file_name))
			return (ERR_SYSTEM);
		return (0);
	}
	else
		return (111999);
}

t_err	is_redir_out(t_token *tk, char c, size_t *idx, t_string *str)
{
	if (c == '>')
	{
		tk->type = TK_REDIR;
		tk->u_data.redir.type = REDIR_OUT;
		tk->u_data.redir.fd = STDOUT_FILENO;
		(*idx)++;
		if (str->content[*idx] == '>')
		{
			tk->u_data.redir.type = REDIR_APPEND;
			(*idx)++;
		}
		while (is_whitespace(str->content[*idx]))
			(*idx)++;
		if (!is_arg(str->content[*idx]))
			return (ERR_SYNTAX_ERROR);
		if (parse_arg(str, idx, &tk->u_data.redir.file_name))
			return (ERR_SYSTEM);
		return (0);
	}
	else
		return (111999);
}

t_err	is_parenthesis_or_nothing(t_token *tk, char c, size_t *idx,
		t_string *str)
{
	if (c == '(')
	{
		tk->type = TK_PAROPEN;
		(*idx)++;
	}
	else if (c == ')')
	{
		tk->type = TK_PARCLOSE;
		(*idx)++;
	}
	else if (c == '&')
	{
		(*idx)++;
		if (str->content[*idx] == '&')
		{
			tk->type = TK_AND;
			(*idx)++;
		}
		else
			tk->type = TK_INVALID;
	}
	else
		return (111999);
	return (ERR_OK);
}

static t_err	add_token(t_token *tk, t_token **token)
{
	*token = malloc(sizeof(t_token));
	if (!*token)
		return (ERR_SYSTEM);
	**token = *tk;
	return (ERR_OK);
}

/// Returns ERR_OK, ERR_SYNTAX_ERROR or ERR_SYSTEM
static t_err	get_token(t_string *str, size_t *idx, t_token **token)
{
	t_token	tk;
	int		ret;

	*token = NULL;
	ret = is_pipe_or_arg(&tk, str->content[*idx], idx, str);
	if (ret == ERR_SYSTEM || ret == ERR_SYNTAX_ERROR)
		return (ret);
	if (ret == ERR_OK)
		return (add_token(&tk, token));
	ret = is_redir_in(&tk, str->content[*idx], idx, str);
	if (ret == ERR_SYSTEM || ret == ERR_SYNTAX_ERROR)
		return (ret);
	if (ret == ERR_OK)
		return (add_token(&tk, token));
	ret = is_redir_out(&tk, str->content[*idx], idx, str);
	if (ret == ERR_SYSTEM || ret == ERR_SYNTAX_ERROR)
		return (ret);
	if (ret == ERR_OK)
		return (add_token(&tk, token));
	ret = is_parenthesis_or_nothing(&tk, str->content[*idx], idx, str);
	if (ret == ERR_SYSTEM || ret == ERR_SYNTAX_ERROR)
		return (ret);
	if (ret == ERR_OK)
		return (add_token(&tk, token));
	return (ERR_OK);
}

/// Returns ERR_OK, ERR_SYNTAX_ERROR or ERR_SYSTEM
t_err	lex(t_string *str, t_list **tokens)
{
	t_list	*new_list;
	t_token	*token;
	size_t	idx;
	t_err	err;

	*tokens = NULL;
	idx = 0;
	while (idx < str->length)
	{
		while (idx < str->length && is_whitespace(str->content[idx]))
			idx++;
		errno = 0;
		err = get_token(str, &idx, &token);
		if (err)
			return (ft_lstclear(tokens, (void (*)(void *))free_token), err);
		if (!token)
			break ;
		new_list = ft_lstnew(token);
		if (!new_list)
			return (ERR_SYSTEM);
		ft_lstadd_back(tokens, new_list);
	}
	return (ERR_OK);
}
