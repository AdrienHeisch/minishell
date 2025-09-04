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

void	builtin_echo(char **args, t_shell_data *shell_data, int fd_out)
{
	int		flags;
	size_t	idx;
	char	*options;

	options = "n";
	idx = 1;
	find_options(&flags, args, &idx, options);
	while (args[idx])
	{
		ft_putstr_fd(args[idx++], fd_out);
		if (args[idx])
			ft_putstr_fd(" ", fd_out);
	}
	if (!(flags & (ft_strchr(options, 'n') - options + 1)))
		ft_putstr_fd("\n", fd_out);
	shell_data->status = 0;
}
