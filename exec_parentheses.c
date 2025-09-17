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

void	exec_parentheses(t_expr *paren, t_shell_data *shell_data)
{
	int	pid;
	int	status_location;

	pid = fork();
	if (pid == -1)
		exit(42);
	if (pid == 0)
	{
		if (resolve_redirections(paren, shell_data))
			exit(1);
		if (dup2(paren->fd_in, STDIN_FILENO) == -1)
			exit(-1);
		if (dup2(paren->fd_out, STDOUT_FILENO) == -1)
			exit(-1);
		exec_expr(paren->data.paren.inner, shell_data);
		close_redirections(paren->fd_in, paren->fd_out);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		free_expr(paren);
		free_shell_data(shell_data);
		exit(shell_data->status);
	}
	waitpid(pid, &status_location, 0);
	if (WIFEXITED(status_location))
		shell_data->status = WEXITSTATUS(status_location);
	else
		shell_data->status = 0;
}
