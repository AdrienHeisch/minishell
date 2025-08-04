/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galauren <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 03:24:35 by galauren          #+#    #+#             */
/*   Updated: 2025/06/27 01:01:58 by galauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	wait_all(void)
{
	while (wait(NULL) > 0)
		;
}

void	exit_error(const char *msg, char **str, char ***tab)
{
	int	len;

	len = 0;
	while (msg[len])
		len++;
	write(2, msg, len);
	write(2, "\n", 1);
	free_choice(str, tab);
	initialize_or_close_files(1, -42, -42, NULL);
	exit(EXIT_FAILURE);
}

int	open_infile(char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		exit_error("open infile", NULL, NULL);
	return (fd);
}

int	open_outfile(char *path)
{
	int	fd;

	fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		exit_error("open outfile", NULL, NULL);
	return (fd);
}

int	count_paths(const char *s)
{
	int	count;

	count = 1;
	while (*s)
		if (*s++ == ':')
			count++;
	return (count);
}
