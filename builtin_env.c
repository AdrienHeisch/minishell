/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:09:20 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/11 19:09:20 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stddef.h>
#include <stdlib.h>

void	builtin_env(char **args, t_shell_data *shell_data, int fd_out)
{
	size_t	env_idx;
	int		flags;
	size_t	idx;

	idx = 1;
	if (find_options(&flags, args, &idx, ""))
	{
		shell_data->status = 2;
		return ;
	}
	env_idx = 0;
	while (shell_data->envp[env_idx])
	{
		ft_putstr_fd(shell_data->envp[env_idx++], fd_out);
		ft_putstr_fd("\n", fd_out);
	}
	shell_data->status = 0;
}
