/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 16:59:39 by aheisch           #+#    #+#             */
/*   Updated: 2025/09/29 16:59:39 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/// Returns ERR_OK or ERR_SYSTEM
static t_err	replace_var(t_string *var, t_shell_data *shell_data)
{
	t_string	var_name;
	char		*value;

	ft_string_move(var, &var_name);
	*var = ft_string_new();
	if (!var->content)
		return (ERR_SYSTEM);
	value = ft_getenv(shell_data->envp, var_name.content);
	if (!value)
		value = "";
	if (!ft_string_cat(var, value))
		return (ERR_SYSTEM);
	ft_string_destroy(&var_name);
	return (ERR_OK);
}

static bool	is_var_name_start_char(char c)
{
	return (ft_isalpha(c) || c == '_');
}

static bool	is_var_name_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/// Returns ERR_OK or ERR_SYSTEM
static t_err	split_var(char *from, t_string *exp, t_list **out)
{
	char	*to;

	while (exp->length == 0 && is_whitespace(*from))
		from++;
	while (true)
	{
		to = ft_strchr(from, ' ');
		if (!to)
			to = ft_strchr(from, '\t');
		if (!to)
			to = ft_strchr(from, '\n');
		if (!to && !ft_string_cat(exp, from))
			return (ERR_SYSTEM);
		if (!to)
			return (ERR_OK);
		if (!ft_string_ncat(exp, from, to - from) || lstadd_back_string(out,
				*exp))
			return (ERR_SYSTEM);
		*exp = ft_string_new();
		if (!exp->content)
			return (ERR_SYSTEM);
		from = to;
		while (is_whitespace(*from))
			from++;
	}
}

/// errno will be set on error
static char	*get_wildcard_pattern(char *s, size_t *len)
{
	char		del;
	t_string	pattern;

	pattern = ft_string_new();
	if (!pattern.content)
		return (NULL);
	del = '\0';
	*len = 0;
	while (s[*len])
	{
		if (!del && is_whitespace(s[*len]))
			break ;
		if (!del && (s[*len] == '\'' || s[*len] == '"'))
		{
			del = s[(*len)++];
			continue ;
		}
		if (s[*len] != del && !ft_string_ncat(&pattern, &s[*len], 1))
			return (NULL);
		if (s[(*len)++] == del)
			del = '\0';
	}
	if (ft_strnchr(s, '*', *len))
		return (pattern.content);
	return (ft_string_destroy(&pattern), NULL);
}

struct						s_expand
{
	t_list					*out;
	t_string				exp;
	char					del;
	size_t					idx;
	bool					has_empty_var;
};

static t_err				expand_var1(struct s_expand *expand, t_string var,
								char *pattern, size_t pattern_len);

static t_err	expand_var(t_string *arg, t_shell_data *shell_data,
		struct s_expand *expand)
{
	t_string	var;
	t_string	pat;
	char		*pattern;
	size_t		len;

	len = 0;
	var = ft_string_new();
	if (!var.content)
		return (ERR_SYSTEM);
	while (expand->idx + len < arg->length
		&& is_var_name_char(arg->content[expand->idx + len]))
		len++;
	if (!ft_string_ncat(&var, &arg->content[expand->idx], len))
		return (ft_string_destroy(&var), ERR_SYSTEM);
	expand->idx += len;
	if (replace_var(&var, shell_data))
		return (ERR_SYSTEM);
	pat = ft_string_new();
	if (!pat.content || !ft_string_cat(&pat, var.content)
		|| !ft_string_cat(&pat, &arg->content[expand->idx]))
		return (ft_string_destroy(&var), ft_string_destroy(&pat), ERR_SYSTEM);
	pattern = get_wildcard_pattern(pat.content, &len);
	if (errno)
		return (ft_string_destroy(&var), ft_string_destroy(&pat), ERR_SYSTEM);
	return (ft_string_destroy(&pat), expand_var1(expand, var, pattern, len));
}

static t_err	expand_var1(struct s_expand *expand, t_string var,
		char *pattern, size_t pattern_len)
{
	t_list	*wildcard;

	if (pattern && expand->del == '\0')
	{
		expand->idx += pattern_len - var.length;
		errno = 0;
		wildcard = expand_wildcards(pattern);
		if (errno)
			return (ERR_SYSTEM);
		if (wildcard)
		{
			while (wildcard)
				ft_lstadd_back(&expand->out, ft_lstpop_front(&wildcard));
			return (ft_string_destroy(&var), free(pattern), ERR_OK);
		}
	}
	if (var.length == 0)
		expand->has_empty_var = true;
	if (var.length != 0 && !expand->del && split_var(var.content, &expand->exp,
			&expand->out))
		return (free(pattern), ERR_SYSTEM);
	if (var.length != 0 && expand->del && !ft_string_cat(&expand->exp,
			var.content))
		return (free(pattern), ERR_SYSTEM);
	return (free(pattern), ft_string_destroy(&var), ERR_OK);
}

static t_err	expand_init(struct s_expand *expand)
{
	expand->out = NULL;
	expand->exp = ft_string_new();
	if (!expand->exp.content)
		return (ERR_SYSTEM);
	expand->del = '\0';
	expand->idx = 0;
	expand->has_empty_var = false;
	return (ERR_OK);
}

static void	expand_free(struct s_expand expand)
{
	ft_string_destroy(&expand.exp);
}

enum						e_control_flow
{
	CF_NONE,
	CF_CONTINUE,
	CF_RETURN_ERR,
};

static t_list				*expand_arg1(struct s_expand expand);

static enum e_control_flow	expand_dollar1(t_string *arg,
		t_shell_data *shell_data, struct s_expand *expand)
{
	char	*status;

	if (arg->content[expand->idx + 1] == '?')
	{
		expand->idx += 2;
		status = ft_itoa(shell_data->status);
		if (!status || !ft_string_cat_free(&expand->exp, status))
			return (CF_RETURN_ERR);
		return (CF_CONTINUE);
	}
	if (arg->content[expand->idx + 1] == '*')
	{
		expand->idx += 2;
		expand->has_empty_var = true;
		return (CF_CONTINUE);
	}
	if (ft_isdigit(arg->content[expand->idx + 1]))
	{
		expand->idx++;
		while (expand->idx < arg->length
			&& ft_isdigit(arg->content[expand->idx]))
			expand->idx++;
		return (CF_CONTINUE);
	}
	return (CF_NONE);
}

static enum e_control_flow	expand_dollar2(t_string *arg,
		t_shell_data *shell_data, struct s_expand *expand, bool is_heredoc)
{
	size_t	len;

	if (!expand->del && (arg->content[expand->idx + 1] == '\'' || (!is_heredoc
			&& arg->content[expand->idx + 1] == '"')))
	{
		expand->del = arg->content[expand->idx + 1];
		expand->idx += 2;
		len = 0;
		while (expand->idx + len < arg->length && arg->content[expand->idx
			+ len] != expand->del)
			len++;
		if (!ft_string_ncat(&expand->exp, &arg->content[expand->idx], len))
			return (CF_RETURN_ERR);
		expand->idx += len;
		return (CF_CONTINUE);
	}
	if (is_var_name_start_char(arg->content[expand->idx + 1]))
	{
		expand->idx++;
		if (expand_var(arg, shell_data, expand))
			return (CF_RETURN_ERR);
		return (CF_CONTINUE);
	}
	return (CF_NONE);
}

static enum e_control_flow	do_expand(t_string *arg, t_shell_data *shell_data,
		struct s_expand *expand, bool is_heredoc)
{
	enum e_control_flow	cf;

	if (!expand->del && (arg->content[expand->idx] == '\'' || (!is_heredoc
			&& arg->content[expand->idx] == '"')))
	{
		expand->del = arg->content[expand->idx++];
		return (CF_CONTINUE);
	}
	if (expand->del && expand->del == arg->content[expand->idx])
	{
		expand->del = '\0';
		expand->idx++;
		expand->has_empty_var = false;
		return (CF_CONTINUE);
	}
	if (arg->content[expand->idx] == '$' && expand->del != '\'' && expand->idx
		+ 1 < arg->length)
	{
		cf = expand_dollar1(arg, shell_data, expand);
		if (!cf)
			cf = expand_dollar2(arg, shell_data, expand, is_heredoc);
		if (cf)
			return (cf);
	}
	return (CF_NONE);
}

static enum e_control_flow	catch_wildcard(t_string *arg,
		struct s_expand *expand)
{
	t_list	*wildcard;
	char	*pattern;
	size_t	pattern_len;

	pattern = get_wildcard_pattern(&arg->content[expand->idx], &pattern_len);
	if (expand->exp.length == 0 && pattern && expand->del == '\0')
	{
		expand->idx += pattern_len;
		errno = 0;
		wildcard = expand_wildcards(pattern);
		if (errno)
			return (free(pattern), CF_RETURN_ERR);
		if (!wildcard && !ft_string_cat(&expand->exp, pattern))
			return (free(pattern), CF_RETURN_ERR);
		while (wildcard)
			ft_lstadd_back(&expand->out, ft_lstpop_front(&wildcard));
		return (free(pattern), CF_CONTINUE);
	}
	return (free(pattern), CF_NONE);
}

/// TODO free all relevant variables on error return
///
/// errno will only be set on error
t_list	*expand_arg(t_string *arg, t_shell_data *shell_data, bool is_heredoc)
{
	struct s_expand		expand;
	enum e_control_flow	cf;

	errno = 0;
	if (expand_init(&expand))
		return (NULL);
	while (expand.idx < arg->length && arg->content[expand.idx])
	{
		cf = do_expand(arg, shell_data, &expand, is_heredoc);
		if (!cf)
			cf = catch_wildcard(arg, &expand);
		if (cf == CF_RETURN_ERR)
			return (expand_free(expand), NULL);
		if (cf == CF_CONTINUE)
			continue ;
		if (!ft_string_ncat(&expand.exp, &arg->content[expand.idx], 1))
			return (expand_free(expand), NULL);
		expand.idx++;
	}
	return (expand_arg1(expand));
}

static t_list	*expand_arg1(struct s_expand expand)
{
	t_string	empty;

	if (expand.exp.length > 0)
	{
		if (lstadd_back_string(&expand.out, expand.exp))
			return (NULL);
	}
	else
		ft_string_destroy(&expand.exp);
	if (ft_lstsize(expand.out) == 0 && !expand.has_empty_var)
	{
		empty = ft_string_new();
		if (!empty.content)
			return (NULL);
		if (lstadd_back_string(&expand.out, empty))
			return (NULL);
	}
	return (expand.out);
}
