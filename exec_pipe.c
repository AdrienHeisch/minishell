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

static void	run_child(t_cmd cmd, char **envp, int in_fd, int out_fd)
{
	if (dup2(in_fd, STDIN_FILENO) == -1)
		exit(42);
	if (dup2(out_fd, STDOUT_FILENO) == -1)
		exit(42);
	// close(in_fd);
	exec_cmd(cmd, envp);
}

static void	child_and_pipe(t_cmd cmd, char **envp, int *prev_fd, int *next_fd)
{
	pid_t	pid;

	if (pipe(next_fd) == -1)
		exit(42);
	pid = fork();
	if (pid == -1)
	{
		close(*prev_fd);
		close(next_fd[0]);
		close(next_fd[1]);
		exit(42);
	}
	if (pid == 0)
	{
		close(next_fd[0]);
		run_child(cmd, envp, *prev_fd, next_fd[1]);
	}
	close(next_fd[1]);
	if (*prev_fd != -1)
		close(*prev_fd);
	*prev_fd = next_fd[0];
}

static void	child_last(t_cmd cmd, char **envp, int prev_fd, int outfile)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		exit(42);
	if (pid == 0)
		run_child(cmd, envp, prev_fd, outfile);
	if (prev_fd != -1)
		close(prev_fd);
	close(outfile);
}

static void	wait_all(void)
{
	while (wait(NULL) > 0)
		;
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

void	exec_pipe(t_pipe pipe, char **envp)
{
	t_list	*cmds;
	t_list	*cmd;
	int		prev_fd;
	int		next_fd[2];

	cmds = NULL;
	build_cmd_list(&cmds, pipe);
	if (!cmds)
		exit(42);
	prev_fd = ((t_cmd *)cmds->content)->fd_in;
	cmd = cmds;
	while (cmd->next != NULL)
	{
		child_and_pipe(*((t_cmd *)cmd->content), envp, &prev_fd, next_fd);
		cmd = cmd->next;
	}
	child_last(*((t_cmd *)cmd->content), envp, prev_fd,
		((t_cmd *)cmd->content)->fd_out);
	wait_all();
}
