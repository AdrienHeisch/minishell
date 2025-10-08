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
		path = ft_getenv(envp, "PWD");
	if (!path || !*path)
		return (NULL);
	dirs = split_path(path, 0);
	if (!dirs)
		return (NULL);
	i = -1;
	while (dirs[++i])
	{
		path = join_path_cmd(dirs[i], cmd);
		if (!path)
			return (free_tab_and_null(&dirs), NULL);
		if (access(path, F_OK) == 0)
			return (free_tab_and_null(&dirs), path);
		free(path);
	}
	free_tab_and_null(&dirs);
	return (NULL);
}

// TODO ambiguous return
int	resolve_exec_path(char **args, t_shell_data *shell_data)
{
	DIR		*dir;
	char	*initial_path;

	if (!args[0])
		return (0);
	if (ft_strlen(args[0]) == 0)
		return (ERR_COMMAND_NOT_FOUND);
	initial_path = args[0];
	if (!ft_strchr(args[0], '/'))
		args[0] = find_cmd_path(args[0], shell_data->envp);
	if (!args[0])
		return (args[0] = initial_path, ERR_COMMAND_NOT_FOUND);
	if (initial_path != args[0])
		free(initial_path);
	dir = opendir(args[0]);
	if (dir != NULL)
		return (closedir(dir), ERR_PERMISSION);
	return (-1);
}
