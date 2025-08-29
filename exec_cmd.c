/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 20:49:31 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/28 20:49:31 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

void	exec_cmd(t_cmd cmd, t_shell_data *shell_data)
{
	int			status_location;
	t_exec_info	exec;

	if (resolve_redirections(&cmd))
	{
		shell_data->status = 1;
		return ;
	}
	exec = make_exec_info(cmd, shell_data);
	if (exec.error >= 0)
	{
		shell_data->status = exec.error;
		return ;
	}
	if (is_builtin(exec.args[0]))
		exec_builtin(exec, shell_data);
	else
	{
		status_location = -1;
		waitpid(fork_exec_cmd(exec, shell_data), &status_location, 0);
		if (WIFEXITED(status_location))
			shell_data->status = WEXITSTATUS(status_location);
		else
			shell_data->status = 0;
	}
	close_redirections(&cmd);
}
