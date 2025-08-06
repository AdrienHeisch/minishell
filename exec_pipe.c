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

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static void	run_child(t_cmd cmd, char **envp, int in_fd, int out_fd)
{
	if (dup2(in_fd, STDIN_FILENO) == -1)
		exit(42);
	if (dup2(out_fd, STDOUT_FILENO) == -1)
		exit(42);
	close(in_fd);
	exec_cmd(cmd, envp);
}

static void	initialize_or_close_files(int close_them, int in, int out, int *pipe)
{
	static int	infile = -42;
	static int	outfile = -42;
	static int	fd[2] = {-42, -42};

	if (close_them)
	{
		close(infile);
		close(outfile);
		close(fd[0]);
		close(fd[1]);
	}
	else
	{
		if (pipe != NULL)
		{
			fd[0] = pipe[0];
			fd[1] = pipe[1];
		}
		else
		{
			infile = in;
			outfile = out;
		}
	}
}

static void	child_and_pipe(t_cmd cmd, char **envp, int *prev_fd, int *next_fd)
{
	pid_t	pid;

	if (pipe(next_fd) == -1)
		exit(42);
	initialize_or_close_files(0, -42, -42, next_fd);
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

void	wait_all(void)
{
	while (wait(NULL) > 0)
		;
}

void	exec_pipe(t_pipe pipe, char **envp)
{
	int		prev_fd;
	int		next_fd[2];

	prev_fd = pipe.left->data.cmd.fd_in;
	child_and_pipe(pipe.left->data.cmd, envp, &prev_fd, next_fd);
	child_last(pipe.right->data.cmd, envp, prev_fd, 1);
	wait_all();
}
