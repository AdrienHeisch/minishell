/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:07:07 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/28 21:19:48 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void	run_child(t_expr *expr, t_shell_data *shell_data)
{
	if (expr->type == EX_CMD)
	{
		t_exec_info exec = make_exec_info(expr->data.cmd, expr->fd_in, expr->fd_out, shell_data);
		if (exec.error >= 0)
			return ;
		run_cmd(exec, shell_data);
		exit(0);
	}
	else if (expr->type == EX_PARENTHESES)
	{
		exec_parentheses(expr, shell_data);
		exit(shell_data->status);
	}
	else
		exit(MS_LOGIC_ERROR);
}

static int	wait_all(int last_pid)
{
	int	status_location;
	int	exit_code;

	status_location = -1;
	if (last_pid != 0)
	{
		waitpid(last_pid, &status_location, 0);
		if (WIFEXITED(status_location))
			exit_code = WEXITSTATUS(status_location);
		else
			exit_code = -1;
	}
	else
		exit_code = -1;
	while (waitpid(0, &status_location, 0) > 0)
		;
	return (exit_code);
}

static void	run_last_child(t_expr *expr, t_shell_data *shell_data)
{
	if (expr->type == EX_CMD)
	{
		t_exec_info exec = make_exec_info(expr->data.cmd,
				expr->fd_in, expr->fd_out,
				shell_data); // HACK PAREN/CMD
		if (exec.error >= 0)
		{
			shell_data->status = exec.error;
			return ;
		}
		shell_data->status = wait_all(fork_run_cmd(exec, shell_data));
	}
	else if (expr->type == EX_PARENTHESES)
	{
		exec_parentheses(expr, shell_data);
		wait_all(0);
	}
	else
		exit(MS_LOGIC_ERROR);
}

static void	child_and_pipe(t_expr *expr, t_shell_data *shell_data, int *prev_fd,
		int *next_fd)
{
	pid_t		pid;

	if (pipe(next_fd) == -1)
		exit(-1);
	// printf("c&p\n");
	pid = fork();
	if (pid == -1)
	{
		close(*prev_fd);
		close(next_fd[0]);
		close(next_fd[1]);
		exit(-1);
	}
	if (pid == 0)
	{
		// printf("child\n");
		close(next_fd[0]);
		if (expr->fd_in == STDIN_FILENO)
			expr->fd_in = *prev_fd;
		if (expr->fd_out == STDOUT_FILENO)
			expr->fd_out = next_fd[1];
		run_child(expr, shell_data);
		exit(MS_UNREACHABLE);
	}
	// printf("parent\n");
	close(next_fd[1]);
	if (*prev_fd > 0)
		close(*prev_fd);
	*prev_fd = next_fd[0];
}

static void	build_pipeline(t_list **pipeline, t_binop pipe)
{
	if (pipe.left->type == EX_BINOP)
	{
		if (pipe.left->data.binop.op == OP_PIPE)
			build_pipeline(pipeline, pipe.left->data.binop);
		else
			exit(MS_LOGIC_ERROR);
	}
	else if (pipe.left->type == EX_CMD || pipe.left->type == EX_PARENTHESES)
		ft_lstadd_back(pipeline, ft_lstnew(pipe.left));
	else
		exit(MS_UNREACHABLE);
	if (pipe.right->type == EX_CMD || pipe.right->type == EX_PARENTHESES)
		ft_lstadd_back(pipeline, ft_lstnew(pipe.right));
	else
		exit(MS_LOGIC_ERROR);
}

void	exec_pipe(t_binop pipe, t_shell_data *shell_data)
{
	t_list	*pipeline;
	t_list	*el;
	int		prev_fd;
	int		next_fd[2];

	pipeline = NULL;
	build_pipeline(&pipeline, pipe);
	if (!pipeline)
		exit(MS_LOGIC_ERROR);
	el = pipeline;
	if (resolve_redirections(el->content))
	{
		shell_data->status = 1;
		return ;
	}
	prev_fd = ((t_expr *)el->content)->fd_in;
	while (el->next != NULL)
	{
		child_and_pipe(((t_expr *)el->content), shell_data, &prev_fd, next_fd);
		el = el->next;
		// FIXME should kill forked processes ?
		if (resolve_redirections(el->content))
		{
			shell_data->status = 1;
			return ;
		}
	}
	if (((t_expr *)el->content)->fd_in == STDIN_FILENO)
		((t_expr *)el->content)->fd_in = prev_fd;
	run_last_child(el->content, shell_data);
	if (prev_fd > 0)
		close(prev_fd);
	ft_lstclear(&pipeline, no_op);
}
