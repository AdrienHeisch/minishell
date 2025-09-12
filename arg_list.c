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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void	expand_var(t_string *var, t_shell_data *shell_data, char del)
{
	t_string	var_name;
	char		*value;

	ft_string_move(var, &var_name);
	*var = ft_string_new();
	value = ft_getenv(shell_data->envp, var_name.content);
	if (!value)
		value = "";
	if (del != '"')
	{
		value = ft_strtrim(value, " \t\n");
		if (!value)
			exit(MS_ALLOC);
	}
	ft_string_cat(var, value);
	if (del != '"')
		free(value);
	ft_string_destroy(&var_name);
}

static bool	is_var_name_start_char(char c)
{
	return (ft_isalpha(c) || c == '_');
}

static bool	is_var_name_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static void	split_var(t_string *var, t_string *exp, t_list **out)
{
	char	*var_from;
	char	*var_to;

	var_from = var->content;
	while (true)
	{
		while (*var_from == ' ')
			var_from++;
		var_to = ft_strchr(var_from, ' ');
		if (!var_to)
		{
			ft_string_cat(exp, var_from);
			break ;
		}
		ft_string_ncat(exp, var_from, var_to - var_from);
		lstadd_back_string(out, *exp);
		*exp = ft_string_new();
		var_from = var_to;
	}
}

static char	*get_wildcard_pattern(char *s, size_t *len)
{
	bool		is_pattern;
	char		del;
	t_string	pattern;
	size_t		idx;

	is_pattern = false;
	pattern = ft_string_new();
	del = '\0';
	idx = 0;
	while (s[idx])
	{
		if (!del)
		{
			if (s[idx] == ' ')
				break ;
			if (s[idx] == '\'' || s[idx] == '"')
			{
				del = s[idx];
				idx++;
				continue ;
			}
		}
		if (s[idx] != del)
			ft_string_ncat(&pattern, &s[idx], 1);
		else if (del)
			del = '\0';
		if (s[idx] == '*')
			is_pattern = true;
		idx++;
	}
	*len = idx;
	if (is_pattern)
		return (pattern.content);
	return (NULL);
}

t_list	*expand_arg(t_string *arg, t_shell_data *shell_data)
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

	out = NULL;
	exp = ft_string_new();
	del = '\0';
	idx = 0;
	has_empty_var = false;
	while (idx < arg->length && arg->content[idx])
	{
		if (!del && (arg->content[idx] == '\'' || arg->content[idx] == '"'))
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
				ft_string_cat(&exp, ft_itoa(shell_data->status));
				continue ;
			}
			if (!del && (arg->content[idx + 1] == '\'' || arg->content[idx
					+ 1] == '"'))
			{
				del = arg->content[idx + 1];
				idx += 2;
				len = 0;
				while (idx + len < arg->length && arg->content[idx
					+ len] != del)
					len++;
				ft_string_ncat(&exp, &arg->content[idx], len);
				idx += len;
				continue ;
			}
			if (is_var_name_start_char(arg->content[idx + 1]))
			{
				idx++;
				len = 0;
				var = ft_string_new();
				while (idx + len < arg->length
					&& is_var_name_char(arg->content[idx + len]))
					len++;
				ft_string_ncat(&var, &arg->content[idx], len);
				expand_var(&var, shell_data, del);
				if (var.length == 0)
					has_empty_var = true;
				else if (del == '\0')
					split_var(&var, &exp, &out);
				else
					ft_string_cat(&exp, var.content);
				ft_string_destroy(&var);
				idx += len;
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
		size_t len_;
		pattern = get_wildcard_pattern(&arg->content[idx], &len_);
		if (pattern && del == '\0')
		{
			idx += len_;
			wildcard = expand_wildcards(pattern);
			if (!wildcard)
				ft_string_cat(&exp, pattern);
			else
			{
				if (exp.length > 0)
				{
					lstadd_back_string(&out, exp);
					exp = ft_string_new();
				}
				while (wildcard)
					ft_lstadd_back(&out, ft_lstpop_front(&wildcard));
			}
			continue ;
		}
		ft_string_ncat(&exp, &arg->content[idx], 1);
		idx++;
	}
	if (exp.length > 0)
		lstadd_back_string(&out, exp);
	if (ft_lstsize(out) == 0 && !has_empty_var)
	{
		t_string empty = ft_string_new();
		lstadd_back_string(&out, empty);
	}
	return (out);
}

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
		lst = expand_arg(&((t_arg_data *)arg_list->content)->string,
				shell_data);
		while (lst)
			ft_lstadd_back(&expanded, ft_lstpop_front(&lst));
		arg_list = arg_list->next;
	}
	args = ft_calloc((ft_lstsize(expanded) + 1), sizeof(char *));
	idx = 0;
	arg_list = expanded;
	while (arg_list && arg_list->content)
	{
		if (((t_arg_data *)arg_list->content)->string.content)
		{
			args[idx] = ft_strdup(((t_string *)arg_list->content)->content);
			idx++;
		}
		arg_list = arg_list->next;
	}
	return (args);
}

void	free_args_list(char **args)
{
	size_t	idx;

	idx = 0;
	while (args[idx])
		free(args[idx++]);
	free(args);
}
