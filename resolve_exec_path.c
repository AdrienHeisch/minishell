/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_exec_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 15:24:57 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/29 15:24:57 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

static int	count_paths(const char *s)
{
	int	count;

	count = 1;
	while (*s)
		if (*s++ == ':')
			count++;
	return (count);
}

static char	*copy_dir(const char *start, int len)
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

static void	free_tab(char ***tab)
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

static char	**split_path(const char *s)
{
	char	**dirs;
	int		i;
	int		start;
	int		j;

	dirs = malloc(sizeof(char *) * (count_paths(s) + 1));
	if (!dirs)
		return (NULL);
	i = 0;
	j = 0;
	start = 0;
	while (s[i++])
	{
		if (s[i] == ':' || s[i + 1] == '\0')
		{
			if (i - start == 0 && ++i)
				continue ;
			if (!s[i + 1])
				i++;
			dirs[j] = copy_dir(&s[start], i - start);
			if (!dirs[j++])
				return (free_tab(&dirs), NULL);
			start = i + 1;
		}
	}
	dirs[j] = NULL;
	return (dirs);
}

static char	*join_path_cmd(char *dir, char *cmd)
{
	int		len1;
	int		len2;
	char	*joined;
	int		i;

	len1 = 0;
	len2 = 0;
	i = 0;
	while (dir[len1])
		len1++;
	while (cmd[len2])
		len2++;
	joined = malloc(len1 + len2 + 2);
	if (!joined)
		return (NULL);
	while (i < len1)
	{
		joined[i] = dir[i];
		i++;
	}
	joined[i++] = '/';
	while (*cmd)
		joined[i++] = *cmd++;
	joined[i] = '\0';
	return (joined);
}

static char	*find_cmd_path(char *cmd, char **envp)
{
	char	**dirs;
	char	*path;
	int		i;

	path = ft_getenv(envp, "PATH");
	if (!path || !*path)
		return (NULL);
	dirs = split_path(path);
	if (!dirs)
		return (NULL);
	i = -1;
	while (dirs[++i])
	{
		path = join_path_cmd(dirs[i], cmd);
		if (!path)
			return (free_tab(&dirs), NULL);
		if (access(path, X_OK) == 0)
		{
			free_tab(&dirs);
			return (path);
		}
		free(path);
	}
	free_tab(&dirs);
	return (NULL);
}

int	resolve_exec_path(char **args, t_shell_data *shell_data)
{
	DIR		*dir;
	char	*initial_path;

	if (!args[0])
		return (0);
	if (ft_strlen(args[0]) == 0)
		return (127);
	initial_path = args[0];
	if (!ft_strchr(args[0], '/') || access(args[0], F_OK) == -1)
	{
		args[0] = find_cmd_path(args[0], shell_data->envp);
	}
	if (!args[0])
	{
		args[0] = initial_path;
		return (127);
	}
	if (initial_path != args[0])
		free(initial_path);
	dir = opendir(args[0]);
	if (dir != NULL)
	{
		closedir(dir);
		return (126);
	}
	return (-1);
}
