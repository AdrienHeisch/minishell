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

static t_err	last_child_cases(t_expr *expr, t_shell_data *shell_data)
{
	t_err		err;
	t_exec_info	exec;
	int			pid;

	err = resolve_redirections(expr, shell_data);
	if (err)
		return (print_error(), ERR_COMMAND_FAILED);
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
	return (ERR_OK);
}

/// Returns ERR_OK, ERR_COMMAND_FAILED or ERR_SYSTEM
static t_err	run_last_child(t_expr *expr, t_shell_data *shell_data)
{
	if (expr->type == EX_CMD)
	{
		return (last_child_cases(expr, shell_data));
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
static t_err	build_pipeline(t_list **pipeline, t_binop pipe)
{
	t_list	*new;

	if (pipe.left->type == EX_BINOP)
	{
		if (pipe.left->u_data.binop.op != OP_PIPE)
			exit(ERR_LOGIC);
		build_pipeline(pipeline, pipe.left->u_data.binop);
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
	if (!(pipe.right->type == EX_CMD || pipe.right->type == EX_PARENTHESES))
		exit(ERR_LOGIC);
	new = ft_lstnew(pipe.right);
	if (!new)
		return (ERR_SYSTEM);
	ft_lstadd_back(pipeline, new);
	return (ERR_OK);
}

void	do_for_each(t_list **el, t_shell_data *shell_data, int *prev_fd,
		int next_fd[2])
{
	while ((*el)->next != NULL)
	{
		fork_and_pipe(((t_expr *)(*el)->content), shell_data, prev_fd, next_fd);
		*el = (*el)->next;
	}
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
	do_for_each(&el, shell_data, &prev_fd, next_fd);
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
