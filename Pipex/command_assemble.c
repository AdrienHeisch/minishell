/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_assemble.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galauren <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 01:18:17 by galauren          #+#    #+#             */
/*   Updated: 2025/06/19 06:00:18 by galauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t				i;
	unsigned const char	*str1;
	unsigned const char	*str2;

	i = 0;
	str1 = (unsigned const char *)s1;
	str2 = (unsigned const char *)s2;
	while (i < n)
	{
		if ((str1[i] != str2[i]) || str1[i] == '\0' || str2[i] == '\0')
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}

char	*get_path_var(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			return (envp[i] + 5);
		}
		i++;
	}
	return (NULL);
}

char	*join_path_cmd(char *dir, char *cmd)
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

char	*find_cmd_path(char *cmd, char **envp)
{
	char	**dirs;
	char	*path;
	int		i;

	path = get_path_var(envp);
	if (!path)
		return (NULL);
	dirs = ft_split_path(path);
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
