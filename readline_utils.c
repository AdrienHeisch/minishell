/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 23:58:48 by aheisch           #+#    #+#             */
/*   Updated: 2025/10/08 23:58:48 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <readline/readline.h>
#include <unistd.h>

t_string	readline_lite(char *prompt)
{
	t_string	line;
	char		c;
	ssize_t		n_read;

	errno = 0;
	if (prompt)
		ft_putstr_fd(prompt, STDERR_FILENO);
	line = ft_string_new();
	if (!line.content)
		return (line);
	while (1)
	{
		n_read = read(STDIN_FILENO, &c, 1);
		if (n_read == -1)
			return (ft_string_destroy(&line), line);
		if (n_read == 0)
			return (ft_string_destroy(&line), line);
		if (c == '\n')
			return (line);
		if (!ft_string_ncat(&line, &c, 1))
			return (ft_string_destroy(&line), line);
	}
}
