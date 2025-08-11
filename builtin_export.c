/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:30:36 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/11 19:30:36 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

void	builtin_export(char **args, t_shell_data *shell_data)
{
	char	**split;

	// TODO list env
	if (!args[1])
		return ;
	split = ft_split(args[1], '=');
	if (!split[0] || !split[1])
		return ;
	ft_setenv(&shell_data->envp, split[0], split[1], true);
	shell_data->status = 0;
}
