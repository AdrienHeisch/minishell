/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parentheses.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 01:54:55 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/29 02:04:55 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static t_err	child_process(t_expr *paren, t_shell_data *shell_data)
{
	t_err	err;

	err = resolve_redirections(paren, shell_data);
	if (err == ERR_SYSTEM)
		print_error();
	if (err)
		return (ERR_COMMAND_FAILED);
	if (dup2(paren->fd_in, STDIN_FILENO) == -1)
		return (print_error(), ERR_COMMAND_FAILED);
	if (dup2(paren->fd_out, STDOUT_FILENO) == -1)
		return (print_error(), ERR_COMMAND_FAILED);
	if (exec_expr(paren->u_data.paren.inner, shell_data))
		shell_data->status = ERR_SYSTEM;
	close_redirections(paren->fd_in, paren->fd_out);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	free_expr(paren);
	free_shell_data(shell_data);
	return (shell_data->status);
}

/// Returns ERR_OK or ERR_SYSTEM
t_err	exec_parentheses(t_expr *paren, t_shell_data *shell_data)
{
	int	pid;
	int	status_location;

	pid = fork();
	if (pid == -1)
	{
		print_error();
		return (ERR_SYSTEM);
	}
	if (pid == 0)
		exit(child_process(paren, shell_data));
	if (waitpid(pid, &status_location, 0) == -1)
		return (ERR_SYSTEM);
	if (WIFEXITED(status_location))
		shell_data->status = WEXITSTATUS(status_location);
	else
		shell_data->status = ERR_OK;
	return (ERR_OK);
}
