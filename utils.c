/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:58:29 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/06 17:58:29 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

void	no_op(void *p)
{
	(void)p;
}

void	lstclear_string(void *str)
{
	ft_string_delete((t_string **)&str);
}

bool	is_whitespace(t_string *str)
{
	size_t	idx;

	idx = 0;
	while (idx < str->length)
	{
		if (str->content[idx] != ' ' && str->content[idx] != '\t'
			&& str->content[idx] != '\n')
			return (false);
		idx++;
	}
	return (true);
}

char	*ft_getenv(char **envp, const char *name)
{
	int		idx;
	size_t	len;
	char	*name_suffix;

	name_suffix = ft_strjoin(name, "=");
	if (!name_suffix)
		exit(MS_ALLOC);
	len = ft_strlen(name_suffix);
	idx = 0;
	while (envp[idx])
	{
		if (ft_strncmp(envp[idx], name_suffix, len) == 0)
			return (free(name_suffix), envp[idx] + len);
		idx++;
	}
	free(name_suffix);
	return (NULL);
}

static void	add_to_env(char ***envp, const char *name, const char *value)
{
	char	**old;
	char	**new;
	char	*name_suffix;
	size_t	len;

	old = *envp;
	len = 0;
	while (old[len])
		len++;
	new = malloc((len + 2) * sizeof(char *));
	if (!new)
		exit(MS_ALLOC);
	ft_memcpy(new, old, len * sizeof(char *));
	name_suffix = ft_strjoin(name, "=");
	if (!name_suffix)
		exit(MS_ALLOC);
	new[len] = ft_strjoin(name_suffix, value);
	if (!new[len])
		exit(MS_ALLOC);
	new[len + 1] = NULL;
	free(name_suffix);
	free(old);
	*envp = new;
}

void	ft_setenv(char ***envp, const char *name, const char *value,
		int overwrite)
{
	char	*name_suffix;
	size_t	len;
	size_t	idx;

	if (!value)
		ft_unsetenv(envp, name);
	if (!ft_getenv(*envp, name))
	{
		add_to_env(envp, name, value);
		return ;
	}
	if (!overwrite)
		return ;
	name_suffix = ft_strjoin(name, "=");
	if (!name_suffix)
		exit(MS_ALLOC);
	len = ft_strlen(name_suffix);
	idx = 0;
	while ((*envp)[idx])
	{
		if (ft_strncmp((*envp)[idx], name_suffix, len) == 0)
		{
			free((*envp)[idx]);
			(*envp)[idx] = ft_strjoin(name_suffix, value);
			if (!(*envp)[idx])
				exit(MS_SUCCESS);
			break ;
		}
		idx++;
	}
}

void	ft_unsetenv(char ***envp, const char *name)
{
	char	**old;
	char	**new;
	char	*name_suffix;
	size_t	len;
	size_t	idx;
	size_t	n_skipped;

	if (!ft_getenv(*envp, name))
		return ;
	old = *envp;
	len = 0;
	while (old[len])
		len++;
	name_suffix = ft_strjoin(name, "=");
	if (!name_suffix)
		exit(MS_ALLOC);
	new = malloc(len * sizeof(char *));
	if (!new)
		exit(MS_ALLOC);
	idx = 0;
	n_skipped = 0;
	while (idx + n_skipped < len)
	{
		if (ft_strncmp(old[idx + n_skipped], name_suffix,
				ft_strlen(name_suffix)) == 0)
		{
			n_skipped++;
			continue ;
		}
		new[idx] = old[idx + n_skipped];
		idx++;
	}
	new[idx] = NULL;
	free(name_suffix);
	free(old);
	*envp = new;
}

void	prompt_heredoc(int fd, int out, char *delim)
{
	char		*line;

	if (!delim)
		return ;
	while (1)
	{
		ft_putstr_fd("> ", STDERR_FILENO);
		line = ft_get_next_line(fd);
		if (!line)
			return ; // TODO error handling
		if (!ft_strncmp(line, delim, ft_strlen(line) - 1))
			return ;
		ft_putstr_fd(line, out);
		free(line);
	}
}

static bool	parse_options(char *arg, int *flags, char *options)
{
	int		new_flags;
	size_t	idx;
	size_t	opt_idx;

	new_flags = 0;
	idx = 1;
	while (arg[idx])
	{
		opt_idx = 0;
		while (options[opt_idx])
		{
			if (arg[idx] == options[opt_idx])
			{
				new_flags |= opt_idx + 1;
				break ;
			}
			opt_idx++;
		}
		if (!options[opt_idx])
			return (false);
		idx++;
	}
	*flags |= new_flags;
	return (true);
}

int	find_options(int *flags, char **args, size_t *idx, char *options)
{
	*flags = 0;
	while (args[*idx] && args[*idx][0] == '-')
	{
		if (args[*idx][1] == '-')
			return (-1);
		if (!parse_options(args[*idx], flags, options))
			return (-1);
		(*idx)++;
	}
	return (0);
}
