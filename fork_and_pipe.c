/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_and_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 23:59:06 by aheisch           #+#    #+#             */
/*   Updated: 2025/10/08 23:59:06 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void	child_pipe_linking(t_expr *expr, int redir_res, int *prev_fd,
		int next_fd[2])
{
	if (redir_res)
		exit(ERR_COMMAND_FAILED);
	close(next_fd[0]);
	if (expr->fd_in == STDIN_FILENO)
		expr->fd_in = *prev_fd;
	else
		close(*prev_fd);
	if (expr->fd_out == STDOUT_FILENO)
		expr->fd_out = next_fd[1];
	else
		close(next_fd[1]);
}

static t_err	fork_error(int *prev_fd, int next_fd[2])
{
	print_error();
	close(*prev_fd);
	close(next_fd[0]);
	close(next_fd[1]);
	return (ERR_SYSTEM);
}

static bool	make_pipe(t_list *el, int next_fd[2])
{
	t_expr			*next_expr;
	t_redir_data	*redir;
	t_list			*list;

	next_expr = NULL;
	if (el->next)
	{
		next_expr = el->next->content;
		list = next_expr->redirs;
		while (list)
		{
			redir = list->content;
			if (redir && (redir->type == REDIR_IN
					|| redir->type == REDIR_HEREDOC))
			{
				next_fd[0] = open("/dev/null", O_RDWR);
				next_fd[1] = open("/dev/null", O_RDWR);
				return (ERR_OK);
			}
			list = list->next;
		}
	}
	if (pipe(next_fd) == -1)
		return (ERR_SYSTEM);
	return (ERR_OK);
}

/// Returns ERR_OK or ERR_SYSTEM
t_err	fork_and_pipe(t_list *el, t_shell_data *shell_data,
		int *prev_fd, int next_fd[2])
{
	pid_t	pid;
	int		redir_res;
	t_expr	*expr;

	expr = el->content;
	if (make_pipe(el, next_fd))
		return (ERR_SYSTEM);
	redir_res = resolve_redirections(expr, shell_data);
	if (redir_res == ERR_SYSTEM)
		print_error();
	pid = fork();
	if (pid == -1)
		return (fork_error(prev_fd, next_fd));
	if (pid == 0)
	{
		ft_lstclear(&el, no_op);
		child_pipe_linking(expr, redir_res, prev_fd, next_fd);
		run_child(expr, shell_data);
		exit(ERR_UNREACHABLE);
	}
	close_redirections(expr->fd_in, expr->fd_out);
	close_redirections(*prev_fd, next_fd[1]);
	*prev_fd = next_fd[0];
	return (ERR_OK);
}
