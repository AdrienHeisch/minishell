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

static bool	parse_options(char *arg, int *flags)
{
	int		new_flags;
	size_t	idx;

	new_flags = 0;
	idx = 1;
	while (arg[idx])
	{
		if (arg[idx] == 'n')
			new_flags |= 1;
		else
			return (false);
		idx++;
	}
	*flags |= new_flags;
	return (true);
}

void	builtin_echo(char **args, t_shell_data *shell_data)
{
	int		flags;
	size_t	idx;

	flags = 0;
	idx = 1;
	while (args[idx] && *args[idx] == '-')
	{
		if (!parse_options(args[idx], &flags))
			break ;
		idx++;
	}
	while (args[idx])
	{
		ft_putstr_fd(args[idx++], 1);
		if (args[idx])
			ft_putstr_fd(" ", 1);
	}
	if (!(flags & 1))
		ft_putstr_fd("\n", 1);
	shell_data->status = 0;
}
