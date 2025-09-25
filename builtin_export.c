/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:30:36 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/11 19:30:36 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <errno.h>
#include <stdlib.h>

#define EXPORT_PREFIX "declare -x"

static int	allowed_char(int c)
{
	return (ft_isalnum(c) || c == '_');
}

/// Returns ERR_OK or ERR_COMMAND_FAILED
static t_err	is_valid_var(char *name)
{
	if (ft_strlen(name) == 0)
		return (ERR_COMMAND_FAILED);
	if (ft_strchr(name, '!'))
		return (ERR_COMMAND_FAILED);
	if (ft_isdigit(name[0]))
		return (ERR_COMMAND_FAILED);
	if (!is_str_all(name, allowed_char))
		return (ERR_COMMAND_FAILED);
	return (ERR_OK);
}

/// Returns ERR_OK or ERR_SYSTEM
t_err	export_var(char ***exported, const char *name)
{
	char	**old;
	char	**new;
	size_t	len;

	old = *exported;
	len = 0;
	while (old[len])
		len++;
	new = malloc((len + 2) * sizeof(char *));
	if (!new)
		return (ERR_SYSTEM);
	ft_memcpy(new, old, len * sizeof(char *));
	new[len] = ft_strdup(name);
	if (!new[len])
		return (ERR_SYSTEM);
	new[len + 1] = NULL;
	free(old);
	*exported = new;
	return (ERR_OK);
}

t_err	builtin_export(char **args, t_shell_data *shell_data, int fd_out)
{
	size_t	idx;
	char	**split;
	char	*value;
	int		valid_var_code;
	int		flags;
	int		status;

	idx = 1;
	if (find_options(&flags, args, &idx, NULL))
		return (ERR_SYNTAX_ERROR);
	if (!args[idx])
	{
		idx = 0;
		while (shell_data->exported[idx])
		{
			ft_putstr_fd(EXPORT_PREFIX, fd_out);
			ft_putstr_fd(" ", fd_out);
			ft_putstr_fd(shell_data->exported[idx], fd_out);
			errno = 0;
			value = ft_getenv(shell_data->envp, shell_data->exported[idx]);
			if (value)
			{
				ft_putstr_fd("=\"", fd_out);
				ft_putstr_fd(value, fd_out);
				ft_putstr_fd("\"", fd_out);
			}
			ft_putstr_fd("\n", fd_out);
			idx++;
		}
		return (ERR_OK);
	}
	status = ERR_OK;
	while (args[idx])
	{
		if (ft_strlen(args[idx]) == 0 || ft_strchr(args[idx], '=') == args[idx])
		{
			print_error_msg("export: not a valid identifier");
			status = ERR_COMMAND_FAILED;
			idx++;
			continue ;
		}
		split = ft_split(args[idx], '=');
		if (!split)
			return (ERR_SYSTEM);
		if (!split[0])
		{
			print_error_msg("export: not a valid identifier");
			status = ERR_COMMAND_FAILED;
			idx++;
			free_tab((void ***)&split);
			continue ;
		}
		valid_var_code = is_valid_var(split[0]);
		if (valid_var_code)
		{
			print_error_msg("export: not a valid identifier");
			status = valid_var_code;
			idx++;
			free_tab((void ***)&split);
			continue ;
		}
		if (split[1])
			ft_setenv(&shell_data->envp, split[0], args[idx]
				+ ft_strlen(split[0]) + 1, true);
		else if (ft_strchr(args[idx], '='))
			ft_setenv(&shell_data->envp, split[0], "", true);
		if (export_var(&shell_data->exported, split[0]))
			return (free_tab((void ***)&split), ERR_SYSTEM);
		free_tab((void ***)&split);
		idx++;
	}
	return (status);
}
