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

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

static void	expand_var(t_string *var, t_shell_data *shell_data)
{
	t_string	var_name;
	char		*value;

	ft_string_move(var, &var_name);
	*var = ft_string_new();
	ft_string_term(&var_name);
	value = ft_getenv(shell_data->envp, var_name.content);
	if (!value)
		value = "";
	ft_string_cat(var, value);
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

static void	expand_arg(t_string *arg, t_shell_data *shell_data)
{
	size_t		idx;
	size_t		len;
	t_string	exp;
	t_string	var;
	char		del;
	bool		has_empty_var;

	exp = ft_string_new();
	del = '\0';
	idx = 0;
	has_empty_var = false;
	while (idx < arg->length)
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
				expand_var(&var, shell_data);
				ft_string_ncat(&exp, var.content, var.length);
				if (var.length == 0)
					has_empty_var = true;
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
		ft_string_ncat(&exp, &arg->content[idx], 1);
		idx++;
	}
	ft_string_destroy(arg);
	if (exp.length == 0 && has_empty_var)
		return (ft_string_destroy(&exp), (void)0);
	*arg = exp;
}

char	**make_arg_list(t_cmd cmd, t_shell_data *shell_data)
{
	char	**args;
	size_t	idx;

	args = ft_calloc((ft_lstsize(cmd.args) + 1), sizeof(char *));
	idx = 0;
	while (cmd.args && cmd.args->content)
	{
		expand_arg(&((t_arg_data *)cmd.args->content)->string, shell_data);
		if (((t_arg_data *)cmd.args->content)->string.content)
		{
			ft_string_term(&((t_arg_data *)cmd.args->content)->string);
			args[idx] = ((t_string *)cmd.args->content)->content;
			idx++;
		}
		cmd.args = cmd.args->next;
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

