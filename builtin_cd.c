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

#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	builtin_cd(char **args, t_shell_data *shell_data)
{
	char	*path;
	char	*old_cwd;
	char	*cwd;
	int		flags;
	size_t	idx;
	char	*new_path;

	idx = 1;
	if (find_options(&flags, args, &idx, ""))
		return (ERR_SYNTAX_ERROR);
	if (args[idx] && args[idx + 1])
		return (print_error("cd: too many arguments"), ERR_COMMAND_FAILED);
	old_cwd = getcwd(NULL, 0);
	path = args[idx];
	if (!path)
	{
		path = ft_getenv(shell_data->envp, "HOME");
		if (!path)
			return (print_error("cd: HOME not set"), free(old_cwd),
				ERR_COMMAND_FAILED);
	}
	if (*path && chdir(path) != 0)
		return (print_error_system("cd"), free(old_cwd), ERR_COMMAND_FAILED);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		print_error_system("cd: error retrieving current directory: getcwd");
	if (old_cwd)
	{
		ft_setenv(&shell_data->envp, "OLDPWD", old_cwd, true);
		free(old_cwd);
	}
	else
		ft_setenv(&shell_data->envp, "OLDPWD", ft_getenv(shell_data->envp,
				"PWD"), true);
	if (cwd)
	{
		ft_setenv(&shell_data->envp, "PWD", cwd, true);
		free(cwd);
	}
	else
	{
		new_path = ft_strjoin(ft_getenv(shell_data->envp, "PWD"), path);
		if (!new_path)
			exit(ERR_SYSTEM);
		ft_setenv(&shell_data->envp, "PWD", new_path, true);
		free(new_path);
	}
	return (ERR_OK);
}
