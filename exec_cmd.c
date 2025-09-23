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

void	exec_cmd(t_expr *expr, t_shell_data *shell_data)
{
	int			status_location;
	t_exec_info	exec;
	t_cmd		cmd;

	if (resolve_redirections(expr, shell_data))
	{
		shell_data->status = ERR_COMMAND_FAILED;
		return ;
	}
	cmd = expr->data.cmd;
	exec = make_exec_info(cmd, expr->fd_in, expr->fd_out, shell_data);
	if (exec.error >= 0)
	{
		shell_data->status = exec.error;
		free_exec_info(&exec);
		close_redirections(expr->fd_in, expr->fd_out);
		return ;
	}
	if (is_builtin(exec.args[0])) // DESIGN also checked in run_cmd
	{
		exec_builtin(exec, shell_data);
	}
	else
	{
		status_location = -1;
		waitpid(fork_run_cmd(exec, shell_data), &status_location, 0);
		if (WIFEXITED(status_location))
			shell_data->status = WEXITSTATUS(status_location);
		else
			shell_data->status = ERR_SUCCESS;
	}
	free_exec_info(&exec);
	close_redirections(expr->fd_in, expr->fd_out);
}
