/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:31:16 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/29 17:31:16 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	run_cmd(t_exec_info cmd, t_shell_data *shell_data)
{
	if (is_builtin(*cmd.args))
	{
		exec_builtin(cmd, shell_data);
		close_redirections(cmd.fd_in, cmd.fd_out);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		free_exec_info(&cmd);
		int status = shell_data->status;
		free_shell_data(shell_data);
		exit(status);
	}
	else
	{
		if (dup2(cmd.fd_in, STDIN_FILENO) == -1)
			exit(ERR_SYSTEM);
		if (dup2(cmd.fd_out, STDOUT_FILENO) == -1)
			exit(ERR_SYSTEM);
		execve(cmd.args[0], cmd.args, shell_data->envp);
		// TODO shell scripts ?
		print_error_code(cmd.args[0], errno);
		close_redirections(cmd.fd_in, cmd.fd_out);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		free_exec_info(&cmd);
		if (errno == EACCES)
			exit(ERR_PERMISSION);
		else
			exit(ERR_COMMAND_NOT_FOUND);
	}
}

int	fork_run_cmd(t_exec_info exec, t_shell_data *shell_data)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		exit(ERR_SYSTEM);
	if (pid == 0)
		run_cmd(exec, shell_data);
	return (pid);
}
