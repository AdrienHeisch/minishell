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
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

/// Returns ERR_OK or ERR_SYSTEM
t_err	exec_cmd(t_expr *expr, t_shell_data *shell_data)
{
	int			status_location;
	t_exec_info	exec;
	t_cmd		cmd;
	int			pid;
	t_err		err;

	err = resolve_redirections(expr, shell_data);
	if (err)
	{
		if (err == ERR_SYSTEM)
			print_error();
		shell_data->status = ERR_COMMAND_FAILED;
		return (ERR_OK);
	}
	cmd = expr->data.cmd;
	exec = make_exec_info(cmd, expr->fd_in, expr->fd_out, shell_data);
	if (!exec.args)
		return (ERR_SYSTEM);
	if (exec.error >= 0)
	{
		shell_data->status = exec.error;
		free_exec_info(&exec);
		close_redirections(expr->fd_in, expr->fd_out);
		return (ERR_OK);
	}
	if (is_builtin(exec.args[0]))
	{
		if (exec_builtin(exec, shell_data))
		{
			free_exec_info(&exec);
			close_redirections(expr->fd_in, expr->fd_out);
			return (ERR_SYSTEM);
		}
	}
	else
	{
		status_location = -1;
		pid = fork_run_cmd(exec, shell_data);
		if (pid == -1)
		{
			free_exec_info(&exec);
			close_redirections(expr->fd_in, expr->fd_out);
			return (ERR_SYSTEM);
		}
		if (waitpid(pid, &status_location, 0) == -1)
			return (ERR_SYSTEM);
		if (WIFEXITED(status_location))
			shell_data->status = WEXITSTATUS(status_location);
		else
			shell_data->status = ERR_OK;
	}
	free_exec_info(&exec);
	close_redirections(expr->fd_in, expr->fd_out);
	return (ERR_OK);
}
