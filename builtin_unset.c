/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:11:34 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/13 19:11:34 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_err	builtin_unset(t_exec_info *cmd, t_shell_data *shell_data)
{
	int		flags;
	size_t	idx;

	idx = 1;
	if (find_options(&flags, cmd->args, &idx, ""))
		return (ERR_SYNTAX_ERROR);
	while (cmd->args[idx])
	{
		if (ft_unsetenv(&shell_data->envp, cmd->args[idx]))
			return (ERR_SYSTEM);
		idx++;
	}
	return (ERR_OK);
}
