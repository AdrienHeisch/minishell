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

t_err	not_a_builtin(t_expr *expr, t_shell_data *shell_data, t_exec_info *exec)
{
	int			status_location;
	int			pid;

	status_location = -1;
	pid = fork_run_cmd(*exec, shell_data);
	if (pid == -1)
	{
		free_exec_info(exec);
		close_redirections(expr->fd_in, expr->fd_out);
		return (ERR_SYSTEM);
	}
	if (waitpid(pid, &status_location, 0) == -1)
		return (ERR_SYSTEM);
	if (WIFEXITED(status_location))
		shell_data->status = WEXITSTATUS(status_location);
	else
		shell_data->status = ERR_OK;
	return (ERR_OK);
}

t_err	cmd_format_check(t_err err, t_expr *expr, t_exec_info *exec,
	t_shell_data *shell_data)
{
	t_cmd		cmd;

	if (err)
	{
		if (err == ERR_SYSTEM)
			print_error();
		shell_data->status = ERR_COMMAND_FAILED;
		return (ERR_OK);
	}
	cmd = expr->u_data.cmd;
	*exec = make_exec_info(cmd, expr->fd_in, expr->fd_out, shell_data);
	if (!exec->args)
		return (ERR_SYSTEM);
	if (exec->error >= 0)
	{
		shell_data->status = exec->error;
		free_exec_info(exec);
		close_redirections(expr->fd_in, expr->fd_out);
		return (ERR_OK);
	}
	return (111999);
}

/// Returns ERR_OK or ERR_SYSTEM
t_err	exec_cmd(t_expr *expr, t_shell_data *shell_data)
{
	t_exec_info	exec;
	int			ret;
	t_err		err;

	err = resolve_redirections(expr, shell_data);
	ret = cmd_format_check(err, expr, &exec, shell_data);
	if (ret == ERR_OK || ret == ERR_SYSTEM)
		return (ret);
	if (is_builtin(exec.args[0]))
	{
		if (exec_builtin(&exec, shell_data))
		{
			free_exec_info(&exec);
			close_redirections(expr->fd_in, expr->fd_out);
			return (ERR_SYSTEM);
		}
	}
	else if (not_a_builtin(expr, shell_data, &exec) != ERR_OK)
		return (ERR_SYSTEM);
	free_exec_info(&exec);
	close_redirections(expr->fd_in, expr->fd_out);
	return (ERR_OK);
}
