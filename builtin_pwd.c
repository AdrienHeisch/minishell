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

#include "minishell.h"

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

void	builtin_pwd(char **args, t_shell_data *shell_data)
{
	if (!parse_args(args))
	{
		shell_data->status = 2;
		return ;
	}
	printf("%s\n", shell_data->pwd);
	shell_data->status = 0;
}
