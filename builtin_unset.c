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

void	builtin_unset(char **args, t_shell_data *shell_data)
{
	if (args[1])
		ft_unsetenv(&shell_data->envp, args[1]);
	shell_data->status = 0;
}
