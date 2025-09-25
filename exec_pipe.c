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
	t_exec_info	exec;
	int			error;

	if (expr->type == EX_CMD)
	{
		exec = make_exec_info(expr->u_data.cmd, expr->fd_in, expr->fd_out,
				shell_data);
		if (!exec.args)
		{
			print_error();
			exit(ERR_SYSTEM);
		}
		if (exec.error >= 0)
		{
			close_redirections(expr->fd_in, expr->fd_out);
			error = exec.error;
			free_exec_info(&exec);
			exit(error);
		}
		run_cmd(exec, shell_data);
		exit(ERR_UNREACHABLE);
	}
	else if (expr->type == EX_PARENTHESES)
	{
		if (exec_parentheses(expr, shell_data))
			exit(ERR_SYSTEM);
		exit(shell_data->status);
	}
	else
		exit(ERR_LOGIC);
}

static t_err	wait_all(int last_pid)
{
	int		status_location;
	t_err	exit_code;

	status_location = -1;
	exit_code = ERR_COMMAND_FAILED;
	if (last_pid != 0)
	{
		if (waitpid(last_pid, &status_location, 0) == -1)
			exit_code = ERR_SYSTEM;
		if (WIFEXITED(status_location))
			exit_code = WEXITSTATUS(status_location);
	}
	while (waitpid(0, &status_location, 0) > 0)
		;
	return (exit_code);
}

/// Returns ERR_OK, ERR_COMMAND_FAILED or ERR_SYSTEM
static t_err	run_last_child(t_expr *expr, t_shell_data *shell_data)
{
	t_err		err;
	t_exec_info	exec;
	int			pid;

	if (expr->type == EX_CMD)
	{
		err = resolve_redirections(expr, shell_data);
		if (err == ERR_SYSTEM)
			print_error();
		if (err)
			return (ERR_COMMAND_FAILED);
		exec = make_exec_info(expr->u_data.cmd, expr->fd_in, expr->fd_out,
				shell_data);
		if (!exec.args)
			return (ERR_SYSTEM);
		if (exec.error >= 0)
		{
			shell_data->status = exec.error;
			free_exec_info(&exec);
			return (ERR_OK);
		}
		pid = fork_run_cmd(exec, shell_data);
		if (pid == -1)
			return (ERR_SYSTEM);
		close_redirections(expr->fd_in, expr->fd_out);
		shell_data->status = wait_all(pid);
		free_exec_info(&exec);
	}
	else if (expr->type == EX_PARENTHESES)
	{
		if (exec_parentheses(expr, shell_data))
			return (ERR_SYSTEM);
		wait_all(0);
	}
	else
		exit(ERR_LOGIC);
	return (ERR_OK);
}

/// Returns ERR_OK or ERR_SYSTEM
static t_err	fork_and_pipe(t_expr *expr, t_shell_data *shell_data,
		int *prev_fd, int next_fd[2])
{
	pid_t	pid;
	int		redir_res;

	if (pipe(next_fd) == -1)
		return (ERR_SYSTEM);
	redir_res = resolve_redirections(expr, shell_data);
	if (redir_res == ERR_SYSTEM)
		print_error();
	pid = fork();
	if (pid == -1)
	{
		print_error();
		close(*prev_fd);
		close(next_fd[0]);
		close(next_fd[1]);
		return (ERR_SYSTEM);
	}
	if (pid == 0)
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
		run_child(expr, shell_data);
		exit(ERR_UNREACHABLE);
	}
	close_redirections(expr->fd_in, expr->fd_out);
	close_redirections(*prev_fd, next_fd[1]);
	*prev_fd = next_fd[0];
	return (ERR_OK);
}

/// Returns ERR_OK or ERR_SYSTEM
static t_err	build_pipeline(t_list **pipeline, t_binop pipe)
{
	t_list	*new;

	if (pipe.left->type == EX_BINOP)
	{
		if (pipe.left->u_data.binop.op == OP_PIPE)
			build_pipeline(pipeline, pipe.left->u_data.binop);
		else
			exit(ERR_LOGIC);
	}
	else if (pipe.left->type == EX_CMD || pipe.left->type == EX_PARENTHESES)
	{
		new = ft_lstnew(pipe.left);
		if (!new)
			return (ERR_SYSTEM);
		ft_lstadd_back(pipeline, new);
	}
	else
		exit(ERR_UNREACHABLE);
	if (pipe.right->type == EX_CMD || pipe.right->type == EX_PARENTHESES)
	{
		new = ft_lstnew(pipe.right);
		if (!new)
			return (ERR_SYSTEM);
		ft_lstadd_back(pipeline, new);
	}
	else
		exit(ERR_LOGIC);
	return (ERR_OK);
}

/// Returns ERR_OK or ERR_SYSTEM
t_err	exec_pipe(t_binop pipe, t_shell_data *shell_data)
{
	t_list	*pipeline;
	t_list	*el;
	int		prev_fd;
	int		next_fd[2];
	t_err	err;

	pipeline = NULL;
	build_pipeline(&pipeline, pipe);
	if (!pipeline)
		return (ERR_SYSTEM);
	el = pipeline;
	prev_fd = ((t_expr *)el->content)->fd_in;
	while (el->next != NULL)
	{
		fork_and_pipe(((t_expr *)el->content), shell_data, &prev_fd, next_fd);
		el = el->next;
	}
	if (((t_expr *)el->content)->fd_in == STDIN_FILENO)
		((t_expr *)el->content)->fd_in = prev_fd;
	err = run_last_child(el->content, shell_data);
	if (err == ERR_SYSTEM)
		return (ERR_SYSTEM);
	if (err)
		shell_data->status = err;
	ft_lstclear(&pipeline, no_op);
	return (ERR_OK);
}
