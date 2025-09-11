/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:31:16 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/29 17:31:16 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

// static char	**make_child_env(t_shell_data *shell_data)
// {
// 	char	**env;
// 	size_t	idx;
// 	char	*value;
//
// 	env = ft_calloc(1, sizeof(char *));
// 	if (!env)
// 		exit(MS_ALLOC);
// 	idx = 0;
// 	while (shell_data->exported[idx])
// 	{
// 		value = ft_getenv(shell_data->envp, shell_data->exported[idx]);
// 		if (value)
// 			ft_setenv(&env, shell_data->exported[idx], value, true);
// 		idx++;
// 	}
// 	return (env);
// }

void	run_cmd(t_exec_info cmd, t_shell_data *shell_data)
{
	if (is_builtin(*cmd.args))
	{
		exec_builtin(cmd, shell_data);
		exit(shell_data->status);
	}
	else
	{
		if (dup2(cmd.fd_in, STDIN_FILENO) == -1)
			exit(-1);
		if (dup2(cmd.fd_out, STDOUT_FILENO) == -1)
			exit(-1);
		if (execve(cmd.args[0], cmd.args, shell_data->envp) == -1)
		{
			print_error_code(cmd.args[0], errno);
			free_args_list(cmd.args);
			exit(126);
		}
		free_args_list(cmd.args);
		exit(0);
	}
}

int	fork_run_cmd(t_exec_info exec, t_shell_data *shell_data)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		exit(-1);
	if (pid == 0)
		run_cmd(exec, shell_data);
	return (pid);
}
