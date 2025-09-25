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
#include <errno.h>
#include <string.h>
#include <unistd.h>

void	print_error_msg(char *err)
{
	t_string	error;

	error = ft_string_new();
	if (!error.content)
		return (print_error(), (void)0);
	if (!ft_string_cat(&error, "minishell: "))
		return (ft_string_destroy(&error), print_error(), (void)0);
	if (!ft_string_cat(&error, err))
		return (ft_string_destroy(&error), print_error(), (void)0);
	if (!ft_string_cat(&error, "\n"))
		return (ft_string_destroy(&error), print_error(), (void)0);
	ft_putstr_fd(error.content, STDERR_FILENO);
	ft_string_destroy(&error);
}

void	print_error(void)
{
	if (!errno)
		return ;
	perror("minishell");
}

void	print_error_prefix(char *prefix)
{
	t_string	err;

	if (!errno)
		return ;
	err.content = NULL;
	if (prefix)
		err = ft_string_new();
	if (err.content && ft_string_cat(&err, "minishell: ") && ft_string_cat(&err,
			prefix))
		perror(err.content);
	else
		perror("minishell");
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
		return (perror("minishell: "), (void)0);
	if (!ft_string_cat(&err, path))
		return (ft_string_destroy(&err), print_error(), (void)0);
	if (!ft_string_cat(&err, ": "))
		return (ft_string_destroy(&err), print_error(), (void)0);
	if (!ft_string_cat(&err, msg))
		return (ft_string_destroy(&err), print_error(), (void)0);
	print_error_msg(err.content);
	ft_string_destroy(&err);
}
