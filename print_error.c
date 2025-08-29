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

#include "minishell.h"
#include <string.h>

void	print_error(char *err)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(err, 2);
	ft_putstr_fd("\n", 2);
}

void	print_error_code(char *path, int code)
{
	t_string	err;

	if (code == 0)
		return ;
	err = ft_string_new();
	ft_string_cat(&err, path);
	ft_string_cat(&err, ": ");
	if (code == 126)
		ft_string_cat(&err, "Is a directory");
	else if (code == 127)
		ft_string_cat(&err, "command not found");
	else
		ft_string_cat(&err, strerror(code));
	print_error(err.content);
	ft_string_destroy(&err);
}
