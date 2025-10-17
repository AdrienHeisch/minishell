/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_child.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 18:14:57 by aheisch           #+#    #+#             */
/*   Updated: 2025/10/17 18:14:57 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

static void	other_child_cases(t_expr *expr, t_shell_data *shell_data)
{
	if (expr->type == EX_PARENTHESES)
	{
		if (exec_parentheses(expr, shell_data))
			exit(ERR_SYSTEM);
		exit(shell_data->status);
	}
	else
		exit(ERR_LOGIC);
}

void	run_child(t_expr *expr, t_shell_data *shell_data)
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
			close_redirections(expr->fd_in, expr->fd_out);
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
	else
		other_child_cases(expr, shell_data);
}
