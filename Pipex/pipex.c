/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galauren <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 01:38:31 by galauren          #+#    #+#             */
/*   Updated: 2025/08/04 15:44:10 by galauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exec_cmd(char *cmd_str, char **envp)
{
	char	**cmd;
	char	*path;

	cmd = split_cmd(cmd_str);
	if (!cmd || !cmd[0])
		exit_error("no given command.", NULL, NULL);
	if (access(cmd[0], X_OK) == -1)
		path = find_cmd_path(cmd[0], envp);
	else
		path = cmd[0];
	if (!path)
	{
		write(2, "command not found: ", 19);
		exit_error(*cmd, NULL, &cmd);
	}
	execve(path, cmd, envp);
	exit_error("execve failed", &path, &cmd);
}

void	run_child(char *cmd, char **envp, int in_fd, int out_fd)
{
	if (dup2(in_fd, STDIN_FILENO) == -1)
		exit_error("dup2 in_fd", NULL, NULL);
	if (dup2(out_fd, STDOUT_FILENO) == -1)
		exit_error("dup2 out_fd", NULL, NULL);
	close(in_fd);
	exec_cmd(cmd, envp);
}

void	child_and_pipe(char *cmd, char **envp, int *prev_fd, int *next_fd)
{
	pid_t	pid;

	if (pipe(next_fd) == -1)
		exit_error("pipe", NULL, NULL);
	initialize_or_close_files(0, -42, -42, next_fd);
	pid = fork();
	if (pid == -1)
	{
		close(*prev_fd);
		close(next_fd[0]);
		close(next_fd[1]);
		exit_error("fork", NULL, NULL);
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

void	child_last(char *cmd, char **envp, int prev_fd, int outfile)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		exit_error("fork", NULL, NULL);
	if (pid == 0)
		run_child(cmd, envp, prev_fd, outfile);
	if (prev_fd != -1)
		close(prev_fd);
	close(outfile);
}

int	pipex(t_cmd *cmd)
{
	int		prev_fd;
	int		next_fd[2];

	prev_fd = cmd->infile;
	while (cmd->next != NULL)
	{
		child_and_pipe(cmd->name, cmd->envp, &prev_fd, next_fd);
		cmd = cmd->next;
	}
	child_last(cmd->name, cmd->envp, prev_fd, outfile);
	wait_all();
	return (0);
}
