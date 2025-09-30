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

/// Sets errno to one in case of overflow
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

t_err	concat_exit_error_string(t_string *error, char **args)
{
	*error = ft_string_new();
	if (!error->content)
		return (ERR_SYSTEM);
	if (!ft_string_cat(error, "exit: "))
		return (ft_string_destroy(error), ERR_SYSTEM);
	if (!ft_string_cat(error, args[1]))
		return (ft_string_destroy(error), ERR_SYSTEM);
	if (!ft_string_cat(error, ": numeric argument required"))
		return (ft_string_destroy(error), ERR_SYSTEM);
	print_error_msg(error->content);
	ft_string_destroy(error);
	return (ERR_OK);
}

t_err	builtin_exit(char **args, t_shell_data *shell_data)
{
	long		exit_code;
	t_string	error;
	int			ret;

	if (isatty(STDERR_FILENO))
		ft_putstr_fd("exit\n", STDERR_FILENO);
	exit_code = 0;
	if (args[1])
	{
		exit_code = checked_atol(args[1]);
		if (errno || !is_fully_numeric(args[1]))
		{
			ret = concat_exit_error_string(&error, args);
			if (!error.content)
				return (ret);
			free_shell_data(shell_data);
			exit(ERR_SYNTAX_ERROR);
		}
		if (args[2])
			return (print_error_msg("exit: too many arguments"),
				ERR_COMMAND_FAILED);
	}
	free_shell_data(shell_data);
	exit(exit_code);
}
