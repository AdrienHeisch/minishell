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
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void	resolve_redirections(t_cmd *cmd)
{
	if (cmd->file_out.content)
		cmd->fd_out = open(cmd->file_out.content, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
}

static void	close_redirections(t_cmd *cmd)
{
	if (cmd->file_out.content)
		close(cmd->fd_out);
}

void	exec(t_expr *expr, t_shell_data *shell_data)
{
	int	status_location;

	if (!expr)
		return ;
	if (expr->type == EX_CMD)
	{
		resolve_redirections(&expr->data.cmd);
		// TODO merge is_builtin exec_builtin
		if (expr->data.cmd.args->content
			&& is_builtin(&((t_arg_data *)expr->data.cmd.args->content)->string))
		{
			exec_builtin(expr->data.cmd, shell_data);
		}
		else
		{
			waitpid(child_last(expr->data.cmd, shell_data, expr->data.cmd.fd_in,
					expr->data.cmd.fd_out), &status_location, 0);
			if (WIFEXITED(status_location))
				shell_data->status = WEXITSTATUS(status_location);
			else
				exit(-1);
		}
		close_redirections(&expr->data.cmd);
	}
	else if (expr->type == EX_PIPE)
		shell_data->status = exec_pipe(expr->data.pipe, shell_data);
	else
		shell_data->status = 0;
}
