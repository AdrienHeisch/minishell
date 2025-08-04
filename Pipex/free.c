/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galauren <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 01:25:33 by galauren          #+#    #+#             */
/*   Updated: 2025/06/27 01:27:30 by galauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"

void	initialize_or_close_files(int close_them, int in, int out, int *pipe)
{
	static int	infile = -42;
	static int	outfile = -42;
	static int	fd[2] = {-42, -42};

	if (close_them)
	{
		close(infile);
		close(outfile);
		close(fd[0]);
		close(fd[1]);
	}
	else
	{
		if (pipe != NULL)
		{
			fd[0] = pipe[0];
			fd[1] = pipe[1];
		}
		else
		{
			infile = in;
			outfile = out;
		}
	}
}

void	free_string(char **str)
{
	if (str && *str)
	{
		free(*str);
		*str = NULL;
	}
}

void	free_tab(char ***tab)
{
	int	i;

	i = -1;
	if (tab && *tab && (*tab)[0] != NULL)
	{
		while ((*tab)[++i])
		{
			free((*tab)[i]);
			(*tab)[i] = NULL;
		}
		free(*tab);
		*tab = NULL;
	}
}

void	free_choice(char **str, char ***tab)
{
	free_string(str);
	free_tab(tab);
}
