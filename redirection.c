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
#include <fcntl.h>
#include <unistd.h>

static int	expand_redir(t_string *file_name, t_shell_data *shell_data)
{
	t_list	*lst;

	lst = expand_arg(file_name, shell_data);
	if (ft_lstsize(lst) > 1)
		return (1);
	if (!lst || !lst->content)
		return (1);
	ft_string_destroy(file_name);
	*file_name = *(t_string *)lst->content;
	return (0);
}

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
		if (expand_redir(&redir->file_name, shell_data))
		{
			print_error("ambiguous redirection");
			redir_list = redir_list->next;
			return (1);
		}
		if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
		{
			if (expr->fd_in != STDIN_FILENO)
				close(expr->fd_in);
			if (redir->type == REDIR_HEREDOC)
			{
				if (pipe(pipe_fds) == -1)
				{
					perror("pipe");
					return (1);
				}
				prompt_heredoc(pipe_fds[1],
					redir->file_name.content, shell_data);
				close(pipe_fds[1]);
				expr->fd_in = pipe_fds[0];
			}
			else
			{
				expr->fd_in = open(redir->file_name.content, O_RDONLY);
				if (expr->fd_in == -1)
				{
					perror("open");
					return (1);
				}
			}
		}
		if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		{
			if (expr->fd_out != STDOUT_FILENO && expr->fd_out != STDERR_FILENO)
				close(expr->fd_out);
			oflag = O_WRONLY | O_CREAT;
			if (redir->type == REDIR_APPEND)
				oflag |= O_APPEND;
			else
				oflag |= O_TRUNC;
			expr->fd_out = open(redir->file_name.content, oflag,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			if (expr->fd_in == -1)
			{
				perror("open");
				return (1);
			}
		}
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
