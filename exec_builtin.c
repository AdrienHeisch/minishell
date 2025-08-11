/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:51:44 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/11 12:58:23 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

bool	exec_builtin(char **args, t_shell_data *shell_data)
{
	char	*cmd;
	size_t	len;

	cmd = args[0];
	len = ft_strlen(cmd);
	if (len == 0)
		return (false);
	if (ft_strncmp("echo", cmd, len) == 0)
		builtin_echo(args, shell_data);
	else
		return (false);
	return (true);
}
