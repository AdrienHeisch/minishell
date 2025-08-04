/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galauren <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 01:02:57 by galauren          #+#    #+#             */
/*   Updated: 2025/06/26 18:20:11 by galauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*copy_dir(const char *start, int len)
{
	char	*dir;
	int		i;

	i = 0;
	dir = malloc(len + 1);
	if (!dir)
		return (NULL);
	while (i < len)
	{
		if (!start[i])
		{
			free(dir);
			return (NULL);
		}
		dir[i] = start[i];
		i++;
	}
	dir[i] = '\0';
	return (dir);
}

char	**ft_split_path(const char *s)
{
	char	**dirs;
	int		i;
	int		start;
	int		j;

	dirs = malloc(sizeof(char *) * (count_paths(s) + 1));
	if (!dirs)
		return (NULL);
	i = -1;
	j = 0;
	start = 0;
	while (s[++i])
	{
		if (s[i] == ':' || s[i + 1] == '\0')
		{
			if (i - start == 0 && ++i)
				continue ;
			dirs[j] = copy_dir(&s[start], i - start);
			if (!dirs[j++])
				return (free_tab(&dirs), NULL);
			start = i + 1;
		}
	}
	dirs[j] = NULL;
	return (dirs);
}

int	count_args(const char *cmd)
{
	int	in_word;
	int	count;

	in_word = 0;
	count = 0;
	while (*cmd)
	{
		if (*cmd == '\'')
		{
			while (*(++cmd) && *cmd != '\'')
				;
			if (*cmd != '\'')
				exit_error("Error: Quote missing.", NULL, NULL);
			cmd++;
			if (in_word == 0)
				count++;
			in_word = 1;
		}
		if (*cmd != ' ' && !in_word && ++count)
			in_word = 1;
		else if (*cmd == ' ')
			in_word = 0;
		cmd++;
	}
	return (count);
}

char	*extract_argument(char *cmd, int *i)
{
	int		start;
	char	*arg;

	start = *i;
	while (cmd[*i] && cmd[*i] == ' ')
		(*i)++;
	start = *i;
	if (cmd[*i] == '\'')
	{
		start++;
		(*i)++;
		while (cmd[*i] && cmd[*i] != '\'')
			(*i)++;
		(*i)++;
	}
	while (cmd[*i] && cmd[*i] != ' ')
		(*i)++;
	if (*i > start)
	{
		arg = copy_dir(&cmd[start], *i - start);
		if (!arg)
			return (NULL);
		return (arg);
	}
	return (NULL);
}

char	**split_cmd(char *cmd)
{
	char	**args;
	int		i;
	int		k;
	char	*arg;

	i = count_args(cmd);
	k = 0;
	if (i == 0)
		exit_error("Error: Empty or invalid argument.", NULL, NULL);
	args = malloc(sizeof(char *) * (i + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (cmd[i])
	{
		arg = extract_argument(cmd, &i);
		args[k++] = arg;
		if (!arg && *args && **args)
			exit_error("EXTRACT ARGS FAILED", NULL, &args);
	}
	args[k] = NULL;
	return (args);
}
