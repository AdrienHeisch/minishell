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
#include <stdio.h>
#include <unistd.h>

void	builtin_cd(char **args, t_shell_data *shell_data)
{
	char	*path;
	char	*pwd;

	path = args[1];
	if (!path)
		path = ft_getenv(shell_data->envp, "HOME");
	if (chdir(path) < 0)
	{
		perror("cd");
		shell_data->status = 1;
		return ;
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		shell_data->status = 1;
		return ;
	}
	ft_setenv(&shell_data->envp, "PWD", pwd, true);
	shell_data->status = 0;
}
