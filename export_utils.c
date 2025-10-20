/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 19:18:03 by aheisch           #+#    #+#             */
/*   Updated: 2025/10/06 19:18:03 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

int	allowed_char(int c)
{
	return (ft_isalnum(c) || c == '_');
}

/// Returns ERR_OK or ERR_COMMAND_FAILED
t_err	is_valid_var(char *name)
{
	if (ft_strlen(name) == 0)
		return (ERR_COMMAND_FAILED);
	if (ft_strchr(name, '!'))
		return (ERR_COMMAND_FAILED);
	if (ft_isdigit(name[0]))
		return (ERR_COMMAND_FAILED);
	if (!is_str_all(name, allowed_char))
		return (ERR_COMMAND_FAILED);
	return (ERR_OK);
}

t_err	export_to_env(char **args, size_t *idx, char **split,
		t_shell_data *shell_data)
{
	char	*current;
	char	*val;

	if (split[0][ft_strlen(split[0]) - 1] == '+')
	{
		split[0][ft_strlen(split[0]) - 1] = '\0';
		current = ft_getenv(shell_data->envp, split[0]);
		if (current)
		{
			val = ft_strjoin(current, split[1]);
			if (!val)
				return (ERR_SYSTEM);
		}
		else
			val = split[1];
		if (ft_setenv(&shell_data->envp, split[0], val, true))
			return (ERR_SYSTEM);
		free(current);
	}
	else if (ft_setenv(&shell_data->envp, split[0], args[*idx]
			+ ft_strlen(split[0]) + 1, true))
		return (ERR_SYSTEM);
	return (ERR_OK);
}
