/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_info.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 15:46:48 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/29 15:46:48 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_last_arg(char **args, t_shell_data *shell_data)
{
	size_t	idx;

	idx = 0;
	while (args[idx] && args[idx + 1])
		idx++;
	if (args[idx])
		ft_setenv(&shell_data->envp, "_", args[idx], true);
}

t_exec_info	make_exec_info(t_cmd cmd, int fd_in, int fd_out, t_shell_data *shell_data)
{
	t_exec_info	exec;
	char		*path;

	exec.args = make_arg_list(cmd, shell_data);
	path = exec.args[0];
	if (is_builtin(exec.args[0]))
		exec.error = -1;
	else
		exec.error = resolve_exec_path(exec.args, shell_data);
	if (exec.error >= 0)
		print_error_code(path, exec.error);
	exec.fd_in = fd_in;
	exec.fd_out = fd_out;
	set_last_arg(exec.args, shell_data);
	return (exec);
}

void	free_exec_info(t_exec_info *exec)
{
	free_tab((void ***)&exec->args);
}
