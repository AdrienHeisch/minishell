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
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char	*get_env_var(char **envp, char *name)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], name, ft_strlen(name)))
			return (envp[i] + ft_strlen(name));
		i++;
	}
	return ("");
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

	path = get_env_var(envp, "PATH=");
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

static void	expand_var(t_string *var, t_shell_data *shell_data)
{
	t_string	var_name;
	char		*value;

	ft_string_move(var, &var_name);
	ft_string_cat(&var_name, "=");
	*var = ft_string_new();
	ft_string_term(&var_name);
	value = get_env_var(shell_data->envp, var_name.content);
	ft_string_cat(var, value);
	ft_string_destroy(&var_name);
}

static bool	is_var_name_start_char(char c)
{
	return (ft_isalpha(c) || c == '_');
}

static bool	is_var_name_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static void	expand_arg(t_string *arg, t_shell_data *shell_data)
{
	size_t		idx;
	size_t		len;
	t_string	exp;
	t_string	var;
	char		del;
	bool		has_empty_var;

	exp = ft_string_new();
	del = '\0';
	idx = 0;
	has_empty_var = false;
	while (idx < arg->length)
	{
		if (!del && (arg->content[idx] == '\'' || arg->content[idx] == '"'))
		{
			del = arg->content[idx];
			idx++;
			continue ;
		}
		if (del && del == arg->content[idx])
		{
			del = '\0';
			idx++;
			has_empty_var = false;
			continue ;
		}
		if (arg->content[idx] == '$' && del != '\'' && idx + 1 < arg->length)
		{
			if (arg->content[idx + 1] == '?')
			{
				idx += 2;
				ft_string_cat(&exp, ft_itoa(shell_data->status));
				continue ;
			}
			if (!del && (arg->content[idx + 1] == '\'' || arg->content[idx
					+ 1] == '"'))
			{
				del = arg->content[idx + 1];
				idx += 2;
				len = 0;
				while (idx + len < arg->length && arg->content[idx
					+ len] != del)
					len++;
				ft_string_ncat(&exp, &arg->content[idx], len);
				idx += len;
				continue ;
			}
			if (is_var_name_start_char(arg->content[idx + 1]))
			{
				idx++;
				len = 0;
				var = ft_string_new();
				while (idx + len < arg->length
					&& is_var_name_char(arg->content[idx + len]))
					len++;
				ft_string_ncat(&var, &arg->content[idx], len);
				expand_var(&var, shell_data);
				ft_string_ncat(&exp, var.content, var.length);
				if (var.length == 0)
					has_empty_var = true;
				ft_string_destroy(&var);
				idx += len;
				continue ;
			}
			if (ft_isdigit(arg->content[idx + 1]))
			{
				idx++;
				while (idx < arg->length && ft_isdigit(arg->content[idx]))
					idx++;
				continue ;
			}
		}
		ft_string_ncat(&exp, &arg->content[idx], 1);
		idx++;
	}
	ft_string_destroy(arg);
	if (exp.length == 0 && has_empty_var)
		return (ft_string_destroy(&exp), (void)0);
	*arg = exp;
}

static void	free_args_list(char **args)
{
	size_t	idx;

	idx = 0;
	while (args[idx])
		free(args[idx++]);
	free(args);
}

static void	cmd_error(char *path, char *err)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	if (err)
	{
		ft_putstr_fd(err, 2);
		ft_putstr_fd("\n", 2);
	}
	else
		perror("");
}

void	exec_cmd(t_cmd cmd, t_shell_data *shell_data)
{
	char	**args;
	size_t	idx;
	char	*path;
	DIR		*dir;

	args = ft_calloc((ft_lstsize(cmd.args) + 1), sizeof(char *));
	idx = 0;
	while (cmd.args && cmd.args->content)
	{
		expand_arg(&((t_arg_data *)cmd.args->content)->string, shell_data);
		if (((t_arg_data *)cmd.args->content)->string.content)
		{
			ft_string_term(&((t_arg_data *)cmd.args->content)->string);
			args[idx] = ((t_string *)cmd.args->content)->content;
			idx++;
		}
		cmd.args = cmd.args->next;
	}
	path = args[0];
	if (!path) 
	{
		free_args_list(args);
		exit(0);
	}
	if (ft_strlen(path) == 0)
	{
		free_args_list(args);
		exit(127);
	}
	if (exec_builtin(args, shell_data))
	{
		free_args_list(args);
		exit(0);
	}
	if (access(path, X_OK) == -1)
		path = find_cmd_path(path, shell_data->envp);
	if (!path)
	{
		cmd_error(args[0], "command not found...");
		free_args_list(args);
		exit(127);
	}
	dir = opendir(path);
	if (dir != NULL)
	{
		closedir(dir);
		cmd_error(path, "Is a directory");
		free_args_list(args);
		exit(126);
	}
	if (execve(path, args, shell_data->envp) == -1)
	{
		cmd_error(path, NULL);
		free_args_list(args);
		exit(126);
	}
	free_args_list(args);
	exit(0);
}
