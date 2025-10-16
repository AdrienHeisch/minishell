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

t_err	print_shelldata(char **args, t_shell_data *shell_data, int fd_out,
		size_t *idx)
{
	char	*value;

	if (!args[*idx])
	{
		*idx = 0;
		while (shell_data->exported[*idx])
		{
			ft_putstr_fd(EXPORT_PREFIX, fd_out);
			ft_putstr_fd(" ", fd_out);
			ft_putstr_fd(shell_data->exported[*idx], fd_out);
			errno = 0;
			value = ft_getenv(shell_data->envp, shell_data->exported[*idx]);
			if (value)
			{
				ft_putstr_fd("=\"", fd_out);
				ft_putstr_fd(value, fd_out);
				ft_putstr_fd("\"", fd_out);
			}
			ft_putstr_fd("\n", fd_out);
			(*idx)++;
		}
		return (ERR_OK);
	}
	return (111999);
}

t_err	split_export(char ***split, int *status, size_t *idx)
{
	int		valid_var_code;

	if ((*split)[0] == NULL)
	{
		print_error_msg("export: not a valid identifier");
		*status = ERR_COMMAND_FAILED;
		(*idx)++;
		return (111999);
	}
	valid_var_code = is_valid_var((*split)[0]);
	if (valid_var_code)
	{
		print_error_msg("export: not a valid identifier");
		*status = valid_var_code;
		(*idx)++;
		return (111999);
	}
	return (0);
}

t_err	export_details_check(int *status, char **args,
	t_shell_data *shell_data, size_t *idx)
{
	char	**split;
	int		ret;

	split = ft_split(args[*idx], '=');
	if (!*split)
		return (ERR_SYSTEM);
	ret = split_export(&split, status, idx);
	if (ret == 111999 || ret == ERR_SYSTEM)
		return (free_tab((void ***)&split), 111999);
	if (split[1])
		ft_setenv(&shell_data->envp, split[0], args[*idx]
			+ ft_strlen(split[0]) + 1, true);
	else if (ft_strchr(args[*idx], '='))
		ft_setenv(&shell_data->envp, split[0], "", true);
	if (export_var(&shell_data->exported, split[0]))
		return (free_tab((void ***)&split), ERR_SYSTEM);
	free_tab((void ***)&split);
	return (ERR_OK);
}

t_err	builtin_export(t_exec_info *cmd, t_shell_data *shell_data)
{
	size_t	idx;
	int		flags;
	int		status;

	idx = 1;
	if (find_options(&flags, cmd->args, &idx, NULL))
		return (ERR_SYNTAX_ERROR);
	if (print_shelldata(cmd->args, shell_data, cmd->fd_out, &idx) == ERR_OK)
		return (ERR_OK);
	status = ERR_OK;
	while (cmd->args[idx])
	{
		if (ft_strlen(cmd->args[idx]) == 0 || ft_strchr(cmd->args[idx],
				'=') == cmd->args[idx])
		{
			print_error_msg("export: not a valid identifier");
			status = ERR_COMMAND_FAILED;
			idx++;
			continue ;
		}
		if (export_details_check(&status, cmd->args, shell_data,
				&idx) == ERR_OK)
			idx++;
	}
	return (status);
}
