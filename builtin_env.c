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

#include "minishell.h"
#include <stddef.h>
#include <stdlib.h>

void	builtin_env(t_shell_data *shell_data)
{
	size_t	idx;

	idx = 0;
	while (shell_data->envp[idx])
		printf("%s\n", shell_data->envp[idx++]);
	shell_data->status = 0;
}
