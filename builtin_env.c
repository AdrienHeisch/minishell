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

t_err	builtin_env(t_exec_info *cmd, t_shell_data *shell_data)
{
	size_t	env_idx;
	int		flags;
	size_t	idx;

	idx = 1;
	if (find_options(&flags, cmd->args, &idx, ""))
		return (ERR_SYNTAX_ERROR);
	env_idx = 0;
	while (shell_data->envp[env_idx])
	{
		ft_putstr_fd(shell_data->envp[env_idx++], cmd->fd_out);
		ft_putstr_fd("\n", cmd->fd_out);
	}
	return (ERR_OK);
}
