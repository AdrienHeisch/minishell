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
#include <unistd.h>

static bool	parse_args(char **args)
{
	size_t	idx;

	idx = 1;
	while (args[idx])
	{
		if (args[idx][0] == '-')
			return (false);
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
	if (!cwd)
	{
		shell_data->status = 1;
		return ;
	}
	ft_putstr_fd(cwd, fd_out);
	ft_putstr_fd("\n", fd_out);
	shell_data->status = 0;
}
