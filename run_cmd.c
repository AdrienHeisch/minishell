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
#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	not_a_builtin_i_guess(t_exec_info cmd, t_shell_data *shell_data)
{
	int	error;

	if (dup2(cmd.fd_in, STDIN_FILENO) == -1)
		exit(ERR_SYSTEM);
	if (dup2(cmd.fd_out, STDOUT_FILENO) == -1)
		exit(ERR_SYSTEM);
	execve(cmd.args[0], cmd.args, shell_data->envp);
	error = errno;
	print_error_prefix(cmd.args[0]);
	close_redirections(cmd.fd_in, cmd.fd_out);
	if (close(STDIN_FILENO))
		(print_error(), exit(ERR_SYSTEM));
	free_exec_info(&cmd);
	free_shell_data(shell_data);
	if (error == EACCES)
		exit(ERR_PERMISSION);
	else
		exit(ERR_COMMAND_NOT_FOUND);
}

void	run_cmd(t_exec_info cmd, t_shell_data *shell_data)
{
	int	status;

	if (is_builtin(*cmd.args))
	{
		if (exec_builtin(&cmd, shell_data))
			print_error();
		close_redirections(cmd.fd_in, cmd.fd_out);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		free_exec_info(&cmd);
		status = shell_data->status;
		free_shell_data(shell_data);
		exit(status);
	}
	else
	{
		not_a_builtin_i_guess(cmd, shell_data);
	}
}

int	fork_run_cmd(t_exec_info exec, t_shell_data *shell_data)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGTERM, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		run_cmd(exec, shell_data);
	}
	return (pid);
}
