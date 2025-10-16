/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:51:44 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/11 12:58:23 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

void	find_and_launch_builtin(t_exec_info *cmd, t_shell_data *shell_data,
	size_t len, char *name)
{
	if (ft_strncmp("cd", name, len) == 0)
		shell_data->status = builtin_cd(cmd, shell_data);
	else if (ft_strncmp("echo", name, len) == 0)
		shell_data->status = builtin_echo(cmd, shell_data);
	else if (ft_strncmp("env", name, len) == 0)
		shell_data->status = builtin_env(cmd, shell_data);
	else if (ft_strncmp("exit", name, len) == 0)
		shell_data->status = builtin_exit(cmd, shell_data);
	else if (ft_strncmp("export", name, len) == 0)
		shell_data->status = builtin_export(cmd, shell_data);
	else if (ft_strncmp("pwd", name, len) == 0)
		shell_data->status = builtin_pwd(cmd, shell_data);
	else if (ft_strncmp("unset", name, len) == 0)
		shell_data->status = builtin_unset(cmd, shell_data);
	else
		exit(ERR_LOGIC);
}

/// Always use is_builtin before calling
///
/// Returns ERR_OK or ERR_SYSTEM
t_err	exec_builtin(t_exec_info *cmd, t_shell_data *shell_data)
{
	char	*name;
	size_t	len;

	name = cmd->args[0];
	len = ft_strlen(name) + 1;
	if (len <= 1)
		return (ERR_OK);
	find_and_launch_builtin(cmd, shell_data, len, name);
	if (shell_data->status == ERR_SYSTEM)
		return (ERR_SYSTEM);
	return (ERR_OK);
}
