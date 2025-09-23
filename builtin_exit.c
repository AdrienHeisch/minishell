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
#include <unistd.h>

static bool	is_fully_numeric(const char *s)
{
	size_t	idx;

	if (!*s)
		return (false);
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
	while ((*nptr >= 9 && *nptr <= 13) || is_whitespace(*nptr))
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
	long		exit_code;
	t_string	error;

	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		ft_putstr_fd("exit\n", 2);
	exit_code = 0;
	if (args[1])
	{
		exit_code = checked_atol(args[1]);
		if (errno != 0 || !is_fully_numeric(args[1]))
		{
			error = ft_string_from("exit: ");
			if (!ft_string_cat(&error, args[1]))
				exit(ERR_ALLOC);
			if (!ft_string_cat(&error, " numeric argument required"))
				exit(ERR_ALLOC);
			print_error(error.content);
			ft_string_destroy(&error);
			free_shell_data(shell_data);
			exit(2);
		}
		if (args[2])
		{
			print_error("exit: too many arguments");
			shell_data->status = 1;
			return ;
		}
	}
	free_shell_data(shell_data);
	exit(exit_code);
}
