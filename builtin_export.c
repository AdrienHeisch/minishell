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

#define EXPORT_PREFIX "declare -x"

void	builtin_export(char **args, t_shell_data *shell_data)
{
	size_t	idx;
	char	**split;

	if (!args[1])
	{
		idx = 0;
		while (shell_data->envp[idx])
			printf("%s %s\n", EXPORT_PREFIX, shell_data->envp[idx++]);
		shell_data->status = 0;
		return ;
	}
	split = ft_split(args[1], '=');
	if (!split[0] || !split[1])
		return ;
	ft_setenv(&shell_data->envp, split[0], split[1], true);
	shell_data->status = 0;
}
