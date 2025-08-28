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
#include <stdlib.h>
#include <sys/wait.h>

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
