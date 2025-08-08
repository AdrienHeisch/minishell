/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:07:14 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/06 14:07:14 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

void	exec(t_expr expr, t_shell_data *shell_data)
{
	int status;

	status = -1;
	if (expr.type == EX_CMD)
	{
		child_last(expr.data.cmd, shell_data, expr.data.cmd.fd_in,
			expr.data.cmd.fd_out);
		waitpid(0, &status, 0);
	}
	else if (expr.type == EX_PIPE)
		status = exec_pipe(expr.data.pipe, shell_data);
	shell_data->status = status;
}
