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

bool	exec_builtin(t_exec_info cmd, t_shell_data *shell_data)
{
	char	*path;
	size_t	len;

	path = cmd.args[0];
	len = ft_strlen(path) + 1;
	if (len == 0)
		return (false);
	if (ft_strncmp("cd", path, len) == 0)
		builtin_cd(cmd.args, shell_data);
	else if (ft_strncmp("echo", path, len) == 0)
		builtin_echo(cmd.args, shell_data, cmd.fd_out);
	else if (ft_strncmp("env", path, len) == 0)
		builtin_env(shell_data, cmd.fd_out);
	else if (ft_strncmp("exit", path, len) == 0)
		builtin_exit(cmd.args, shell_data);
	else if (ft_strncmp("export", path, len) == 0)
		builtin_export(cmd.args, shell_data, cmd.fd_out);
	else if (ft_strncmp("pwd", path, len) == 0)
		builtin_pwd(cmd.args, shell_data, cmd.fd_out);
	else if (ft_strncmp("unset", path, len) == 0)
		builtin_unset(cmd.args, shell_data);
	else
		return (false);
	return (true);
}
