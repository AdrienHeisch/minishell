/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:04:45 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/06 13:04:45 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char	*get_path_var(char **envp)
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

static char	**split_path(const char *s)
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

	path = get_path_var(envp);
	if (!path)
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

void	exec_cmd(t_cmd cmd, char **envp)
{
	char	**args;
	size_t	idx;
	char	*path;

	args = malloc(sizeof(char *) * (ft_lstsize(cmd.args) + 1));
	idx = 0;
	while (cmd.args && cmd.args->content)
	{
		ft_string_term(cmd.args->content);
		args[idx] = ((t_string *)cmd.args->content)->content;
		idx++;
		cmd.args = cmd.args->next;
	}
	path = args[0];
	if (access(path, X_OK) == -1)
		path = find_cmd_path(path, envp);
	if (!path)
	{
		ft_putstr_fd("command not found", 2);
		exit(42);
	}
	execve(path, args, envp);
	ft_putstr_fd("execve failed", 2);
	exit(42);
}

// void	exec_cmd(char *cmd_str, char **envp)
// {
// 	char	**cmd;
// 	char	*path;
//
// 	cmd = split_cmd(cmd_str);
// 	if (!cmd || !cmd[0])
// 		exit_error("no given command.", NULL, NULL);
// 	if (access(cmd[0], X_OK) == -1)
// 		path = find_cmd_path(cmd[0], envp);
// 	else
// 		path = cmd[0];
// 	if (!path)
// 	{
// 		write(2, "command not found: ", 19);
// 		exit_error(*cmd, NULL, &cmd);
// 	}
// 	execve(path, cmd, envp);
// 	exit_error("execve failed", &path, &cmd);
// }
