/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 23:03:40 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/13 23:03:40 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

t_err	builtin_cd(char **args, t_shell_data *shell_data)
{
	char		*path;
	char		*old_cwd;
	char		*cwd;
	int			flags;
	size_t		idx;

	idx = 1;
	if (find_options(&flags, args, &idx, ""))
		return (ERR_SYNTAX_ERROR);
	if (args[idx] && args[idx + 1])
		return (print_error_msg("cd: too many arguments"), ERR_COMMAND_FAILED);
	old_cwd = getcwd(NULL, 0);
	if (!old_cwd)
		return (print_error_prefix("cd: error retrieving current directory: getcwd"), ERR_COMMAND_FAILED);
	path = args[idx];
	if (!path)
	{
		errno = 0;
		path = ft_getenv(shell_data->envp, "HOME");
		if (!path)
			return (print_error_msg("cd: HOME not set"), free(old_cwd),
				ERR_COMMAND_FAILED);
	}
	if (*path && chdir(path))
		return (print_error_prefix("cd"), free(old_cwd), ERR_COMMAND_FAILED);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (print_error_prefix("cd: error retrieving current directory: getcwd"), ERR_COMMAND_FAILED);
	ft_setenv(&shell_data->envp, "OLDPWD", old_cwd, true);
	free(old_cwd);
	ft_setenv(&shell_data->envp, "PWD", cwd, true);
	free(cwd);
	return (ERR_OK);
}
