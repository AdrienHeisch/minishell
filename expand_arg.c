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

/// TODO free all relevant variables on error return
///
/// errno will only be set on error
static t_list				*expand_arg1(struct s_expand expand);

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
			return (ft_string_destroy(&expand.exp), NULL);
		if (cf == CF_CONTINUE)
			continue ;
		if (!ft_string_ncat(&expand.exp, &arg->content[expand.idx], 1))
			return (ft_string_destroy(&expand.exp), NULL);
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
