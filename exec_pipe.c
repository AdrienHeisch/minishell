/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:07:07 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/28 21:19:48 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void	run_child(t_exec_info cmd, t_shell_data *shell_data)
{
	if (*cmd.args && is_builtin(*cmd.args))
	{
		exec_builtin(cmd, shell_data);
		exit(shell_data->status);
	}
	else
	{
		if (dup2(cmd.fd_in, STDIN_FILENO) == -1)
			exit(-1);
		if (dup2(cmd.fd_out, STDOUT_FILENO) == -1)
			exit(-1);
		run_cmd(cmd, shell_data);
	}
}

static void	child_and_pipe(t_cmd cmd, t_shell_data *shell_data, int *prev_fd,
		int *next_fd)
{
	pid_t		pid;
	t_exec_info	exec;

	exec = make_exec_info(cmd, shell_data);
	if (exec.error >= 0)
		return ;
	if (pipe(next_fd) == -1)
		exit(-1);
	pid = fork();
	if (pid == -1)
	{
		close(*prev_fd);
		close(next_fd[0]);
		close(next_fd[1]);
		exit(-1);
	}
	if (pid == 0)
	{
		close(next_fd[0]);
		if (exec.fd_in == STDIN_FILENO)
			exec.fd_in = *prev_fd;
		if (exec.fd_out == STDOUT_FILENO)
			exec.fd_out = next_fd[1];
		run_child(exec, shell_data);
	}
	close(next_fd[1]);
	if (*prev_fd > 0)
		close(*prev_fd);
	*prev_fd = next_fd[0];
}

int	fork_exec_cmd(t_exec_info exec, t_shell_data *shell_data)
{
	pid_t		pid;

	pid = fork();
	if (pid == -1)
		exit(-1);
	if (pid == 0)
		run_child(exec, shell_data);
	return (pid);
}

static int	wait_all(int last_pid)
{
	int	status_location;
	int	exit_code;

	status_location = -1;
	waitpid(last_pid, &status_location, 0);
	if (WIFEXITED(status_location))
		exit_code = WEXITSTATUS(status_location);
	else
		exit_code = -1;
	while (waitpid(0, &status_location, 0) > 0)
		;
	return (exit_code);
}

static void	build_cmd_list(t_list **cmds, t_binop pipe)
{
	if (pipe.left->type == EX_BINOP)
	{
		if (pipe.left->data.binop.op == OP_PIPE)
			build_cmd_list(cmds, pipe.left->data.binop);
		else
			exit(42);
	}
	else if (pipe.left->type == EX_CMD)
		ft_lstadd_back(cmds, ft_lstnew(&pipe.left->data.cmd));
	else
		exit(MS_UNREACHABLE);
	if (pipe.right->type == EX_CMD)
		ft_lstadd_back(cmds, ft_lstnew(&pipe.right->data.cmd));
}

void	exec_pipe(t_binop pipe, t_shell_data *shell_data)
{
	t_list	*cmds;
	t_list	*cmd;
	int		prev_fd;
	int		next_fd[2];
	int		exit_code;

	cmds = NULL;
	build_cmd_list(&cmds, pipe);
	if (!cmds)
		exit(-1);
	cmd = cmds;
	if (resolve_redirections(cmd->content))
	{
		shell_data->status = 1;
		return ;
	}
	prev_fd = ((t_cmd *)cmd->content)->fd_in;
	while (cmd->next != NULL)
	{
		child_and_pipe(*((t_cmd *)cmd->content), shell_data, &prev_fd, next_fd);
		cmd = cmd->next;
		// FIXME should kill forked processes ?
		if (resolve_redirections(cmd->content))
		{
			shell_data->status = 1;
			return ;
		}
	}
	if (((t_cmd *)cmd->content)->fd_in == STDIN_FILENO)
		((t_cmd *)cmd->content)->fd_in = prev_fd;
	t_exec_info exec = make_exec_info(*(t_cmd *)cmd->content, shell_data);
	if (exec.error >= 0)
	{
		shell_data->status = exec.error;
		return ;
	}
	exit_code = wait_all(fork_exec_cmd(exec, shell_data));
	if (prev_fd > 0)
		close(prev_fd);
	ft_lstclear(&cmds, no_op);
	shell_data->status = exit_code;
}
