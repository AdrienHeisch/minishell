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

void	exec(t_expr *expr, t_shell_data *shell_data)
{
	int	status_location;

	if (!expr)
		return ;
	if (expr->type == EX_CMD)
	{
		child_last(expr->data.cmd, shell_data, expr->data.cmd.fd_in,
			expr->data.cmd.fd_out);
		waitpid(0, &status_location, 0);
		if (WIFEXITED(status_location))
			shell_data->status = WEXITSTATUS(status_location);
		else
			shell_data->status = -1;
	}
	else if (expr->type == EX_PIPE)
		shell_data->status = exec_pipe(expr->data.pipe, shell_data);
	else
		shell_data->status = 0;
}
