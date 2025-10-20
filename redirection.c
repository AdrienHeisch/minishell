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

#include "libft.h"
#include "minishell.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

/// Returns ERR_OK, ERR_COMMAND_FAILED or ERR_SYSTEM
static t_err	redirin_or_heredoc(t_expr *expr, t_shell_data *shell_data,
		t_redir_data *redir, int pipe_fds[2])
{
	if (redir->type == REDIR_HEREDOC)
	{
		if (pipe(pipe_fds) == -1)
		{
			print_error_prefix("pipe");
			return (close_redirections(expr->fd_in, expr->fd_out),
				ERR_COMMAND_FAILED);
		}
		if (prompt_heredoc(pipe_fds[0], pipe_fds[1], redir->file_name.content,
				shell_data))
			return (ERR_SYSTEM);
		close(pipe_fds[1]);
		expr->fd_in = pipe_fds[0];
	}
	else
	{
		expr->fd_in = open(redir->file_name.content, O_RDONLY);
		if (expr->fd_in == -1)
		{
			print_error_prefix("open");
			return (close_redirections(expr->fd_in, expr->fd_out),
				ERR_COMMAND_FAILED);
		}
	}
	return (ERR_OK);
}

static t_err	redirout_or_append(t_expr *expr, t_redir_data *redir)
{
	int	oflag;

	if (expr->fd_out != STDOUT_FILENO && expr->fd_out != STDERR_FILENO)
		close(expr->fd_out);
	oflag = O_WRONLY | O_CREAT;
	if (redir->type == REDIR_APPEND)
		oflag |= O_APPEND;
	else
		oflag |= O_TRUNC;
	expr->fd_out = open(redir->file_name.content, oflag,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (expr->fd_out == -1)
	{
		print_error_prefix("open");
		return (close_redirections(expr->fd_in, expr->fd_out),
			ERR_COMMAND_FAILED);
	}
	return (ERR_OK);
}

t_err	do_one_redir(t_redir_data *redir, t_shell_data *shell_data,
		t_expr *expr, int pipe_fds[2])
{
	t_err	err;

	if (redir->type != REDIR_HEREDOC)
	{
		err = expand_redir(&redir->file_name, shell_data);
		if (err == ERR_COMMAND_FAILED)
			print_error_msg("ambiguous redirect");
		if (err)
			return (close_redirections(expr->fd_in, expr->fd_out), err);
	}
	if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
	{
		if (expr->fd_in != STDIN_FILENO)
			close(expr->fd_in);
		err = redirin_or_heredoc(expr, shell_data, redir, pipe_fds);
		if (err)
			return (err);
	}
	if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
	{
		err = redirout_or_append(expr, redir);
		if (err)
			return (err);
	}
	return (ERR_OK);
}

/// Returns ERR_OK, ERR_COMMAND_FAILED or ERR_SYSTEM
t_err	resolve_redirections(t_expr *expr, t_shell_data *shell_data)
{
	t_list			*redir_list;
	t_redir_data	*redir;
	t_err			err;
	int				pipe_fds[2];

	(void)pipe_fds;
	errno = 0;
	redir_list = expr->redirs;
	while (redir_list && redir_list->content)
	{
		redir = redir_list->content;
		err = do_one_redir(redir, shell_data, expr, pipe_fds);
		if (err != ERR_OK)
			return (err);
		redir_list = redir_list->next;
	}
	return (ERR_OK);
}

/// errno will be preserved
void	close_redirections(int fd_in, int fd_out)
{
	int	errno_before;

	errno_before = errno;
	errno = 0;
	if (fd_in != -1 && fd_in != STDIN_FILENO)
		close(fd_in);
	if (errno)
		print_error();
	if (fd_out != -1 && fd_out != STDOUT_FILENO)
		close(fd_out);
	if (errno)
		print_error();
	errno = errno_before;
}
