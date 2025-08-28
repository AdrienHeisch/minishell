/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 20:49:31 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/28 20:49:31 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

void	exec_cmd(t_cmd cmd, t_shell_data *shell_data)
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
