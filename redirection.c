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

int	resolve_redirections(t_expr *expr, t_shell_data *shell_data)
{
	int				oflag;
	t_list			*redir_list;
	t_redir_data	*redir;
	int				pipe_fds[2];

	redir_list = expr->redirs;
	while (redir_list)
	{
		redir = redir_list->content;
		expand_arg(&redir->file_name, shell_data);
		if (redir->file_name.length == 0 || ft_strchr(redir->file_name.content, ' '))
		{
			print_error("ambiguous redirection");
			redir_list = redir_list->next;
			continue ;
		}
		if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
		{
			if (expr->fd_in != 0)
				close(expr->fd_in);
			if (redir->type == REDIR_HEREDOC)
			{
				if (pipe(pipe_fds) == -1)
				{
					perror("pipe");
					return (1); // TODO return or continue ?
				}
				prompt_heredoc(STDIN_FILENO, pipe_fds[1],
					redir->file_name.content);
				close(pipe_fds[1]);
				expr->fd_in = pipe_fds[0];
			}
			else
			{
				expr->fd_in = open(redir->file_name.content, O_RDONLY);
				if (expr->fd_in == -1)
				{
					perror("open");
					return (1); // TODO return or continue ?
				}
			}
		}
		else
			expr->fd_in = STDIN_FILENO;
		if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		{
			oflag = O_WRONLY | O_CREAT;
			if (redir->type == REDIR_APPEND)
				oflag |= O_APPEND;
			else
				oflag |= O_TRUNC;
			if (expr->fd_out != 1 && expr->fd_out != 1)
				close(expr->fd_out);
			expr->fd_out = open(redir->file_name.content, oflag,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			if (expr->fd_in == -1)
			{
				perror("open");
				return (1); // TODO return or continue ?
			}
		}
		else
			expr->fd_out = STDOUT_FILENO;
		redir_list = redir_list->next;
	}
	return (0);
}

void	close_redirections(t_expr *expr)
{
	if (expr->fd_in != STDIN_FILENO)
		close(expr->fd_in);
	if (expr->fd_out != STDOUT_FILENO)
		close(expr->fd_out);
}
