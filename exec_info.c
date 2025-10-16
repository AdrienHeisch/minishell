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
#include <stdlib.h>

/// Returns ERR_OK or ERR_SYSTEM
static t_err	set_last_arg(char **args, t_shell_data *shell_data)
{
	size_t	idx;

	idx = 0;
	while (args[idx] && args[idx + 1])
		idx++;
	if (args[idx] && ft_setenv(&shell_data->envp, "_", args[idx], true))
		return (ERR_SYSTEM);
	return (ERR_OK);
}

// TODO make error handling clearer (-1 should not be OK !)

/// .args will be null and errno will be set on execution error.
/// .error will be -1 if there is no command resolution error
t_exec_info	make_exec_info(t_cmd cmd, int fd_in, int fd_out,
		t_shell_data *shell_data)
{
	t_exec_info	exec;
	char		*path;

	exec.args = make_arg_list(cmd, shell_data);
	if (!exec.args)
		return (exec);
	path = NULL;
	if (exec.args[0])
		path = ft_strdup(exec.args[0]);
	if (is_builtin(exec.args[0]))
		exec.error = -1;
	else
		exec.error = resolve_exec_path(exec.args, shell_data);
	if (exec.error > 0)
		print_error_code(path, exec.error);
	free(path);
	exec.fd_in = fd_in;
	exec.fd_out = fd_out;
	if (set_last_arg(exec.args, shell_data))
		return (free_tab((void ***)&exec.args), exec);
	return (exec);
}

void	free_exec_info(t_exec_info *exec)
{
	free_tab((void ***)&exec->args);
}
