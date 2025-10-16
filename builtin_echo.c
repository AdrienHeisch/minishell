/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:48:40 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/11 12:48:40 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

t_err	builtin_echo(t_exec_info *cmd, t_shell_data *shell_data)
{
	int		flags;
	size_t	idx;
	char	*options;

	(void)shell_data;
	options = "n";
	idx = 1;
	find_options(&flags, cmd->args, &idx, options);
	while (cmd->args[idx])
	{
		ft_putstr_fd(cmd->args[idx++], cmd->fd_out);
		if (cmd->args[idx])
			ft_putstr_fd(" ", cmd->fd_out);
	}
	if (!(flags & (ft_strchr(options, 'n') - options + 1)))
		ft_putstr_fd("\n", cmd->fd_out);
	return (ERR_OK);
}
