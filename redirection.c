/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 20:21:14 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/28 20:21:14 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
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

