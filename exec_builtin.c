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

bool	exec_builtin(char **args, t_shell_data *shell_data)
{
	char	*cmd;
	size_t	len;

	cmd = args[0];
	len = ft_strlen(cmd) + 1;
	if (len == 0)
		return (false);
	if (ft_strncmp("cd", cmd, len) == 0)
		builtin_cd(args, shell_data);
	else if (ft_strncmp("echo", cmd, len) == 0)
		builtin_echo(args, shell_data);
	else if (ft_strncmp("env", cmd, len) == 0)
		builtin_env(shell_data);
	else if (ft_strncmp("exit", cmd, len) == 0)
		builtin_exit(args, shell_data);
	else if (ft_strncmp("export", cmd, len) == 0)
		builtin_export(args, shell_data);
	else if (ft_strncmp("pwd", cmd, len) == 0)
		builtin_pwd(args, shell_data);
	else if (ft_strncmp("unset", cmd, len) == 0)
		builtin_unset(args, shell_data);
	else
		return (false);
	return (true);
}
