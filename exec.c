/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:07:14 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/27 16:23:37 by galauren         ###   ########.fr       */
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
	t_string		heredoc;

	redir_list = cmd->redirs;
	while (redir_list)
	{
		redir = redir_list->content;
		if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
		{
			if (cmd->fd_in != 0)
				close(cmd->fd_in);
			if (redir->type == REDIR_HEREDOC)
			{
				cmd->fd_in = open("/tmp/heredoc", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
				heredoc = prompt_heredoc(0, redir->file_name.content);
				write(cmd->fd_in, heredoc.content, heredoc.length + 1);
				close(cmd->fd_in);
				cmd->fd_in = open("/tmp/heredoc", O_RDONLY);
			}
			else
			{
				cmd->fd_in = open(redir->file_name.content, O_RDONLY);
				if (cmd->fd_in == -1)
				{
					perror("open");
					return (1); // TODO return or continue ?
				}
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
				return (1); // TODO return or continue ?
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

void	exec_one(t_cmd cmd, t_shell_data *shell_data)
{
	int	status_location;

	if (resolve_redirections(&cmd))
	{
		shell_data->status = 1;
		return ;
	}
	if (cmd.args && cmd.args->content
		&& is_builtin(&((t_arg_data *)cmd.args->content)->string))
	{
		exec_builtin(cmd, shell_data);
	}
	else
	{
		waitpid(fork_exec_cmd(cmd, shell_data), &status_location,
			0);
		if (WIFEXITED(status_location))
			shell_data->status = WEXITSTATUS(status_location);
		else
			shell_data->status = 0;
	}
	close_redirections(&cmd);
}

void	exec(t_expr *expr, t_shell_data *shell_data)
{
	if (!expr)
		return ;
	if (expr->type == EX_CMD)
		exec_one(expr->data.cmd, shell_data);
	else if (expr->type == EX_PIPE)
		shell_data->status = exec_pipe(expr->data.pipe, shell_data);
	else if (expr->type == EX_LIST)
		exec_expr_list(expr->data.expr_list, shell_data);
	else
		exit(MS_UNREACHABLE);
}
