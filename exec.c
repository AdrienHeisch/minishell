/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:07:14 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/06 14:07:14 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int	resolve_redirections(t_cmd *cmd)
{
	int	oflag;
	t_list	*redir_list;
	t_redir_data *redir;

	redir_list = cmd->redirs;
	while (redir_list)
	{
		redir = redir_list->content;
		if (redir->type == REDIR_IN)
		{
			if (cmd->fd_in != 0)
				close(cmd->fd_in);
			cmd->fd_in = open(redir->file_name.content, O_RDONLY);
			if (cmd->fd_in == -1)
			{
				perror("open");
				return (1); // TODO return or break ?
			}
		}
		else
			cmd->fd_in = STDIN_FILENO;
		if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		{
			oflag = O_WRONLY | O_CREAT;
			if (redir->type == REDIR_APPEND)
				oflag |= O_APPEND;
			else
				oflag |= O_TRUNC;
			if (cmd->fd_out != 1 && cmd->fd_out != 1)
				close(cmd->fd_out);
			cmd->fd_out = open(redir->file_name.content, oflag,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			if (cmd->fd_in == -1)
			{
				perror("open");
				return (1); // TODO return or break ?
			}
		}
		else
			cmd->fd_out = STDOUT_FILENO;
		redir_list = redir_list->next;
	}
	return (0);
}

void	close_redirections(t_cmd *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
}

void	exec(t_expr *expr, t_shell_data *shell_data)
{
	int	status_location;

	if (!expr)
		return ;
	if (expr->type == EX_CMD)
	{
		if (resolve_redirections(&expr->data.cmd))
		{
			shell_data->status = 1;
			return ;
		}
		if (expr->data.cmd.args->content
			&& is_builtin(&((t_arg_data *)expr->data.cmd.args->content)->string))
		{
			exec_builtin(expr->data.cmd, shell_data);
		}
		else
		{
			waitpid(fork_exec_cmd(expr->data.cmd, shell_data), &status_location,
				0);
			if (WIFEXITED(status_location))
				shell_data->status = WEXITSTATUS(status_location);
			else
				exit(-1);
		}
		close_redirections(&expr->data.cmd);
	}
	else if (expr->type == EX_PIPE)
		shell_data->status = exec_pipe(expr->data.pipe, shell_data);
	else
		shell_data->status = 0;
}
