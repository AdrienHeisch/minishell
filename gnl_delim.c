/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_delim.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galauren <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 14:33:34 by galauren          #+#    #+#             */
/*   Updated: 2025/08/27 15:12:31 by galauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <unistd.h>

t_string	gnl_delim(int fd, char *delim)
{
	t_string	ret;
	char		*line;

	ret = ft_string_new();
	if (!delim)
		return (ft_string_destroy(&ret), ret);
	while (1)
	{
		write (1, "> ", 2);
		line = ft_get_next_line(fd);
		if (!line)
			return (ft_string_destroy(&ret), ret);
		if (!ft_strncmp(line, delim, ft_strlen(line) - 1))
			return (free(line), ret);
		ft_string_cat(&ret, line);
		free(line);
	}
}
