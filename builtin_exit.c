/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 23:57:55 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/13 23:57:55 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

static bool	is_fully_numeric(const char *s)
{
	size_t	idx;

	idx = 0;
	if (s[idx] == '-' || s[idx] == '+')
		idx++;
	while (s[idx])
	{
		if (!ft_isdigit(s[idx]))
			return (false);
		idx++;
	}
	return (true);
}

static long	checked_atol(const char *nptr)
{
	long	n;
	int		sign;
	long	add;

	sign = 1;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == ' ')
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	n = 0;
	errno = 0;
	while (ft_isdigit(*nptr))
	{
		add = n * 9 + (*nptr - '0') * sign;
		if ((sign == 1 && LONG_MAX - n < add) || (sign == -1 && LONG_MIN
				- n > add))
			errno = 1;
		n += add;
		nptr++;
	}
	return (n);
}

void	builtin_exit(char **args, t_shell_data *shell_data)
{
	long	exit_code;

	ft_putstr_fd("exit\n", 2);
	exit_code = 0;
	if (args[1])
	{
		exit_code = checked_atol(args[1]);
		if (errno != 0 || !is_fully_numeric(args[1]))
		{
			ft_putstr_fd("exit: ", 2);
			ft_putstr_fd(args[1], 2);
			ft_putstr_fd(" numeric argument required\n", 2);
			exit(2);
		}
		if (args[2])
		{
			ft_putstr_fd("exit: too many arguments\n", 2);
			shell_data->status = 1;
			return ;
		}
	}
	exit(exit_code);
}
