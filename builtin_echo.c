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

void	builtin_echo(char **args, t_shell_data *shell_data)
{
	bool	opt_n;
	size_t	idx;

	opt_n = false;
	idx = 1;
	while (args[idx] && args[idx][0] == '-')
	{
		if (args[idx][1] == 'n')
			opt_n = true;
		idx++;
	}
	while (args[idx])
	{
		ft_putstr_fd(args[idx++], 1);
		if (args[idx])
			ft_putstr_fd(" ", 1);
	}
	if (!opt_n)
		ft_putstr_fd("\n", 1);
	shell_data->status = 0;
}
