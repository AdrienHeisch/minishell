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

static bool	parse_args(char **args)
{
	size_t	idx;

	idx = 1;
	while (args[idx])
	{
		if (args[idx][0] == '-')
		{
			if (args[idx][1] == '-')
			{
				if (args[idx][2])
					return (false);
			}
			else if (args[idx][1])
				return (false);
		}
		idx++;
	}
	return (true);
}

void	builtin_pwd(char **args, t_shell_data *shell_data, int fd_out)
{
	char	*cwd;

	if (!parse_args(args))
	{
		shell_data->status = 2;
		return ;
	}
	cwd = getcwd(NULL, 0);
	if (cwd)
		ft_setenv(&shell_data->envp, "PWD", cwd, true);
	else
		cwd = ft_getenv(shell_data->envp, "PWD");
	ft_putstr_fd(cwd, fd_out);
	ft_putstr_fd("\n", fd_out);
	shell_data->status = 0;
}
