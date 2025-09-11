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
#include <unistd.h>

void	builtin_cd(char **args, t_shell_data *shell_data)
{
	char	*path;
	char	*old_cwd;
	char	*cwd;

	if (args[1] && args[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		shell_data->status = 1;
		return ;
	}
	old_cwd = getcwd(NULL, 0);
	path = args[1];
	if (!path)
	{
		path = ft_getenv(shell_data->envp, "HOME");
		if (!path)
		{
			print_error("cd: HOME not set");
			shell_data->status = 1;
			return ;
		}
	}
	if (*path && chdir(path) != 0)
	{
		perror("cd");
		shell_data->status = 1;
		return ;
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
		perror("cd: error retrieving current directory: getcwd");
	if (old_cwd)
		ft_setenv(&shell_data->envp, "OLDPWD", old_cwd, true);
	else
		ft_setenv(&shell_data->envp, "OLDPWD", ft_getenv(shell_data->envp, "PWD"), true);
	if (cwd)
		ft_setenv(&shell_data->envp, "PWD", cwd, true);
	else
		ft_setenv(&shell_data->envp, "PWD", ft_strjoin(ft_getenv(shell_data->envp, "PWD"), path), true);
	shell_data->status = 0;
}
