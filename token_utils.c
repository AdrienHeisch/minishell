#include "minishell.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

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
t_err	get_token(t_string *str, size_t *idx, t_token **token)
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
