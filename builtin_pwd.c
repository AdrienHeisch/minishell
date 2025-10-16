/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:30:08 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/13 20:30:08 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

t_err	builtin_pwd(t_exec_info *cmd, t_shell_data *shell_data)
{
	char	*cwd;
	int		flags;
	size_t	idx;

	(void)shell_data;
	idx = 1;
	if (find_options(&flags, cmd->args, &idx, ""))
		return (ERR_SYNTAX_ERROR);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (print_error_prefix("pwd: failed to get current directory: \
getcwd"), ERR_COMMAND_FAILED);
	ft_putstr_fd(cwd, cmd->fd_out);
	ft_putstr_fd("\n", cmd->fd_out);
	free(cwd);
	return (ERR_OK);
}
