/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:14:39 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/29 16:14:39 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

void	print_error(char *err)
{
	t_string	error;

	error = ft_string_new();
	if (!ft_string_cat(&error, "minishell: "))
		exit(ERR_SYSTEM);
	if (!ft_string_cat(&error, err))
		exit(ERR_SYSTEM);
	if (!ft_string_cat(&error, "\n"))
		exit(ERR_SYSTEM);
	ft_putstr_fd(error.content, STDERR_FILENO);
}

void	print_error_system(char *prefix)
{
	t_string	err;

	err = ft_string_new();
	if (!err.content)
		exit(ERR_SYSTEM);
	if (prefix && !ft_string_cat(&err, prefix))
		exit(ERR_SYSTEM);
	if (!ft_string_cat(&err, ": "))
		exit(ERR_SYSTEM);
	if (!ft_string_cat(&err, strerror(errno)))
		exit(ERR_SYSTEM);
	print_error(err.content);
	ft_string_destroy(&err);
}

void	print_error_code(char *path, int code)
{
	t_string	err;
	char		*msg;

	if (code == 0)
		return ;
	else if (code == ERR_PERMISSION)
		msg = "Is a directory";
	else if (code == ERR_COMMAND_NOT_FOUND)
		msg = "command not found";
	else
		msg = strerror(code);
	err = ft_string_new();
	if (!err.content)
		exit(ERR_SYSTEM);
	if (!ft_string_cat(&err, path))
		exit(ERR_SYSTEM);
	if (!ft_string_cat(&err, ": "))
		exit(ERR_SYSTEM);
	if (!ft_string_cat(&err, msg))
		exit(ERR_SYSTEM);
	print_error(err.content);
	ft_string_destroy(&err);
}
