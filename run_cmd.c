/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:04:45 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/28 20:47:49 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

void	run_cmd(t_exec_info cmd, t_shell_data *shell_data)
{
	if (execve(cmd.args[0], cmd.args, shell_data->envp) == -1)
	{
		print_error_code(cmd.args[0], errno);
		free_args_list(cmd.args);
		exit(126);
	}
	free_args_list(cmd.args);
	exit(0);
}
