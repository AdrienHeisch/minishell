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

int	builtin_unset(char **args, t_shell_data *shell_data)
{
	int		flags;
	size_t	idx;

	idx = 1;
	if (find_options(&flags, args, &idx, ""))
		return (ERR_SYNTAX_ERROR);
	while (args[idx])
	{
		ft_unsetenv(&shell_data->envp, args[idx]);
		idx++;
	}
	return (ERR_SUCCESS);
}
