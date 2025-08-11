/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:07:07 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/06 14:07:07 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void	run_child(t_cmd cmd, t_shell_data *shell_data, int in_fd, int out_fd)
{
	if (dup2(in_fd, STDIN_FILENO) == -1)
		exit(-1);
	if (dup2(out_fd, STDOUT_FILENO) == -1)
		exit(-1);
	// close(in_fd);
	exec_cmd(cmd, shell_data);
}

static void	child_and_pipe(t_cmd cmd, t_shell_data *shell_data, int *prev_fd, int *next_fd)
{
	pid_t	pid;

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
		run_child(cmd, shell_data, *prev_fd, next_fd[1]);
	}
	close(next_fd[1]);
	if (*prev_fd > 0)
		close(*prev_fd);
	*prev_fd = next_fd[0];
}

void	child_last(t_cmd cmd, t_shell_data *shell_data, int prev_fd, int outfile)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		exit(-1);
	if (pid == 0)
		run_child(cmd, shell_data, prev_fd, outfile);
	if (prev_fd > 0)
		close(prev_fd);
}

static int	wait_all(void)
{
	int	status_location;

	status_location = -1;
	while (waitpid(0, &status_location, 0) > 0)
		;
	if (WIFEXITED(status_location))
		return (WEXITSTATUS(status_location));
	else
		return (-1);
}

static void	build_cmd_list(t_list **cmds, t_pipe pipe)
{
	if (pipe.left->type == EX_PIPE)
		build_cmd_list(cmds, pipe.left->data.pipe);
	else if (pipe.left->type == EX_CMD)
		ft_lstadd_back(cmds, ft_lstnew(&pipe.left->data.cmd));
	if (pipe.right->type == EX_CMD)
		ft_lstadd_back(cmds, ft_lstnew(&pipe.right->data.cmd));
}

static void	no_op(void *el)
{
	(void)el;
}

int	exec_pipe(t_pipe pipe, t_shell_data *shell_data)
{
	t_list	*cmds;
	t_list	*cmd;
	int		prev_fd;
	int		next_fd[2];

	cmds = NULL;
	build_cmd_list(&cmds, pipe);
	if (!cmds)
		exit(-1);
	prev_fd = ((t_cmd *)cmds->content)->fd_in;
	cmd = cmds;
	while (cmd->next != NULL)
	{
		child_and_pipe(*((t_cmd *)cmd->content), shell_data, &prev_fd, next_fd);
		cmd = cmd->next;
	}
	child_last(*((t_cmd *)cmd->content), shell_data, prev_fd,
		((t_cmd *)cmd->content)->fd_out);
	ft_lstclear(&cmds, no_op);
	return (wait_all());
}
