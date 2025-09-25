/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 21:21:42 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/11 21:21:42 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/// Returns ERR_OK or ERR_SYSTEM
static t_err	expand_var(t_string *var, t_shell_data *shell_data)
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
static t_err	split_var(t_string *var, t_string *exp, t_list **out)
{
	char	*var_from;
	char	*var_to;

	var_from = var->content;
	if (exp->length == 0)
	{
		while (is_whitespace(*var_from))
			var_from++;
	}
	while (true)
	{
		var_to = ft_strchr(var_from, ' ');
		if (!var_to)
			var_to = ft_strchr(var_from, '\t');
		if (!var_to)
			var_to = ft_strchr(var_from, '\n');
		if (!var_to)
		{
			if (!ft_string_cat(exp, var_from))
				return (ERR_SYSTEM);
			break ;
		}
		if (!ft_string_ncat(exp, var_from, var_to - var_from))
			return (ERR_SYSTEM);
		if (lstadd_back_string(out, *exp))
			return (ERR_SYSTEM);
		*exp = ft_string_new();
		if (!exp->content)
			return (ERR_SYSTEM);
		var_from = var_to;
		while (is_whitespace(*var_from))
			var_from++;
	}
	return (ERR_OK);
}

/// errno will be set on error
static char	*get_wildcard_pattern(char *s, size_t *len)
{
	bool		is_pattern;
	char		del;
	t_string	pattern;
	size_t		idx;

	is_pattern = false;
	pattern = ft_string_new();
	if (!pattern.content)
		return (NULL);
	del = '\0';
	idx = 0;
	while (s[idx])
	{
		if (!del)
		{
			if (is_whitespace(s[idx]))
				break ;
			if (s[idx] == '\'' || s[idx] == '"')
			{
				del = s[idx];
				idx++;
				continue ;
			}
		}
		if (s[idx] != del)
		{
			if (!ft_string_ncat(&pattern, &s[idx], 1))
				return (NULL);
		}
		else if (del)
			del = '\0';
		if (s[idx] == '*')
			is_pattern = true;
		idx++;
	}
	*len = idx;
	if (is_pattern)
		return (pattern.content);
	ft_string_destroy(&pattern);
	return (NULL);
}

/// TODO free all relevant variables on error return
///
/// errno will only be set on error
t_list	*expand_arg(t_string *arg, t_shell_data *shell_data, bool is_heredoc)
{
	size_t		idx;
	size_t		len;
	t_string	exp;
	t_string	var;
	char		del;
	bool		has_empty_var;
	t_list		*out;
	t_list		*wildcard;
	char		*pattern;
	char		*status;
	t_string	potential_pattern;
	size_t		pattern_len;
	t_string	empty;

	errno = 0;
	out = NULL;
	exp = ft_string_new();
	if (!exp.content)
		return (NULL);
	del = '\0';
	idx = 0;
	has_empty_var = false;
	while (idx < arg->length && arg->content[idx])
	{
		if (!del && (arg->content[idx] == '\'' || (!is_heredoc
				&& arg->content[idx] == '"')))
		{
			del = arg->content[idx];
			idx++;
			continue ;
		}
		if (del && del == arg->content[idx])
		{
			del = '\0';
			idx++;
			has_empty_var = false;
			continue ;
		}
		if (arg->content[idx] == '$' && del != '\'' && idx + 1 < arg->length)
		{
			if (arg->content[idx + 1] == '?')
			{
				idx += 2;
				status = ft_itoa(shell_data->status);
				if (!status)
					return (NULL);
				if (!ft_string_cat_free(&exp, status))
					return (NULL);
				continue ;
			}
			if (arg->content[idx + 1] == '*')
			{
				idx += 2;
				has_empty_var = true;
				continue ;
			}
			if (!del && (arg->content[idx + 1] == '\'' || (!is_heredoc
					&& arg->content[idx + 1] == '"')))
			{
				del = arg->content[idx + 1];
				idx += 2;
				len = 0;
				while (idx + len < arg->length && arg->content[idx
					+ len] != del)
					len++;
				if (!ft_string_ncat(&exp, &arg->content[idx], len))
					return (NULL);
				idx += len;
				continue ;
			}
			if (is_var_name_start_char(arg->content[idx + 1]))
			{
				idx++;
				len = 0;
				var = ft_string_new();
				if (!var.content)
					return (NULL);
				while (idx + len < arg->length
					&& is_var_name_char(arg->content[idx + len]))
					len++;
				if (!ft_string_ncat(&var, &arg->content[idx], len))
					return (NULL);
				idx += len;
				if (expand_var(&var, shell_data))
					return (NULL);
				potential_pattern = ft_string_new();
				if (!potential_pattern.content)
					return (NULL);
				if (!ft_string_cat(&potential_pattern, var.content))
					return (NULL);
				if (!ft_string_cat(&potential_pattern, &arg->content[idx]))
					return (NULL);
				pattern = get_wildcard_pattern(potential_pattern.content,
						&pattern_len);
				if (errno)
					return (NULL);
				ft_string_destroy(&potential_pattern);
				if (pattern && del == '\0')
				{
					idx += pattern_len - var.length;
					errno = 0;
					wildcard = expand_wildcards(pattern);
					if (errno)
						return (NULL);
					if (wildcard)
					{
						while (wildcard)
							ft_lstadd_back(&out, ft_lstpop_front(&wildcard));
						ft_string_destroy(&var);
						free(pattern);
						continue ;
					}
				}
				free(pattern);
				if (var.length == 0)
					has_empty_var = true;
				else if (del == '\0')
				{
					if (split_var(&var, &exp, &out))
						return (NULL);
				}
				else
				{
					if (!ft_string_cat(&exp, var.content))
						return (NULL);
				}
				ft_string_destroy(&var);
				continue ;
			}
			if (ft_isdigit(arg->content[idx + 1]))
			{
				idx++;
				while (idx < arg->length && ft_isdigit(arg->content[idx]))
					idx++;
				continue ;
			}
		}
		pattern = get_wildcard_pattern(&arg->content[idx], &pattern_len);
		if (exp.length == 0 && pattern && del == '\0')
		{
			idx += pattern_len;
			errno = 0;
			wildcard = expand_wildcards(pattern);
			if (errno)
				return (NULL);
			if (!wildcard)
			{
				if (!ft_string_cat_free(&exp, pattern))
					return (NULL);
			}
			while (wildcard)
				ft_lstadd_back(&out, ft_lstpop_front(&wildcard));
			continue ;
		}
		free(pattern);
		if (!ft_string_ncat(&exp, &arg->content[idx], 1))
			return (NULL);
		idx++;
	}
	if (exp.length > 0)
	{
		if (lstadd_back_string(&out, exp))
			return (NULL);
	}
	else
		ft_string_destroy(&exp);
	if (ft_lstsize(out) == 0 && !has_empty_var)
	{
		empty = ft_string_new();
		if (!empty.content)
			return (NULL);
		if (lstadd_back_string(&out, empty))
			return (NULL);
	}
	return (out);
}

/// Will return null and errno will be set on error
char	**make_arg_list(t_cmd cmd, t_shell_data *shell_data)
{
	char	**args;
	size_t	idx;
	t_list	*arg_list;
	t_list	*expanded;
	t_list	*lst;

	expanded = NULL;
	arg_list = cmd.args;
	while (arg_list && arg_list->content)
	{
		errno = 0;
		lst = expand_arg(&((t_arg_data *)arg_list->content)->string, shell_data,
				false);
		if (errno)
			return (ft_lstclear(&expanded, lstclear_string), ft_lstclear(&lst,
					lstclear_string), NULL);
		while (lst)
			ft_lstadd_back(&expanded, ft_lstpop_front(&lst));
		arg_list = arg_list->next;
	}
	args = ft_calloc((ft_lstsize(expanded) + 1), sizeof(char *));
	if (!args)
		return (ft_lstclear(&expanded, lstclear_string), NULL);
	idx = 0;
	arg_list = expanded;
	while (arg_list && arg_list->content)
	{
		args[idx] = ft_strdup(((t_arg_data *)arg_list->content)->string.content);
		if (!args[idx])
			return (ft_lstclear(&expanded, lstclear_string),
				free_tab((void ***)&args), NULL);
		idx++;
		arg_list = arg_list->next;
	}
	ft_lstclear(&expanded, lstclear_string);
	return (args);
}
