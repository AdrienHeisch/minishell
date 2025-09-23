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

void	print_error(char *err)
{
	t_string	error;

	error = ft_string_from("minishell: ");
	if (!ft_string_cat(&error, err))
		exit(ERR_ALLOC);
	if (!ft_string_cat(&error, "\n"))
		exit(ERR_ALLOC);
	ft_putstr_fd(error.content, STDERR_FILENO);
}

void	print_error_code(char *path, int code)
{
	t_string	err;

	if (code == 0)
		return ;
	err = ft_string_new();
	if (!err.content)
		exit(ERR_ALLOC);
	if (!ft_string_cat(&err, path))
		exit(ERR_ALLOC);
	if (!ft_string_cat(&err, ": "))
		exit(ERR_ALLOC);
	if (code == 126)
	{
		if (!ft_string_cat(&err, "Is a directory"))
			exit(ERR_ALLOC);
	}
	else if (code == 127)
	{
		if (!ft_string_cat(&err, "command not found"))
			exit(ERR_ALLOC);
	}
	else
	{
		if (!ft_string_cat(&err, strerror(code)))
			exit(ERR_ALLOC);
	}
	print_error(err.content);
	ft_string_destroy(&err);
}
