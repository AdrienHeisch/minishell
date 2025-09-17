/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:30:36 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/11 19:30:36 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

#define EXPORT_PREFIX "declare -x"

static int	allowed_char(int c)
{
	return (ft_isalnum(c) || c == '_');
}

static int	is_valid_var(char *name)
{
	if (ft_strlen(name) == 0)
		return (1);
	if (ft_strchr(name, '!'))
		return (1);
	if (ft_isdigit(name[0]))
		return (1);
	if (!is_str_all(name, allowed_char))
		return (1);
	return (-1);
}

void	export_var(char ***exported, const char *name)
{
	char	**old;
	char	**new;
	size_t	len;

	old = *exported;
	len = 0;
	while (old[len])
		len++;
	new = malloc((len + 2) * sizeof(char *));
	if (!new)
		exit(MS_ALLOC);
	ft_memcpy(new, old, len * sizeof(char *));
	new[len] = ft_strdup(name);
	if (!new[len])
		exit(MS_ALLOC);
	new[len + 1] = NULL;
	free(old);
	*exported = new;
}

void	builtin_export(char **args, t_shell_data *shell_data, int fd_out)
{
	size_t	idx;
	char	**split;
	char	*value;
	int		valid_var_code;
	int		flags;

	shell_data->status = 0;
	idx = 1;
	if (find_options(&flags, args, &idx, NULL))
	{
		shell_data->status = 2;
		return ;
	}
	if (!args[idx])
	{
		idx = 0;
		while (shell_data->exported[idx])
		{
			ft_putstr_fd(EXPORT_PREFIX, fd_out);
			ft_putstr_fd(" ", fd_out);
			ft_putstr_fd(shell_data->exported[idx], fd_out);
			value = ft_getenv(shell_data->envp, shell_data->exported[idx]);
			if (value)
			{
				ft_putstr_fd("=\"", fd_out);
				ft_putstr_fd(value, fd_out);
				ft_putstr_fd("\"", fd_out);
			}
			ft_putstr_fd("\n", fd_out);
			idx++;
		}
		return ;
	}
	while (args[idx])
	{
		if (ft_strlen(args[idx]) == 0 || ft_strchr(args[idx], '=') == args[idx])
		{
			shell_data->status = 1;
			idx++;
			continue ;
		}
		split = ft_split(args[idx], '=');
		if (!split[0])
		{
			shell_data->status = 1;
			idx++;
			free_tab((void **)split);
			continue ;
		}
		valid_var_code = is_valid_var(split[0]);
		if (valid_var_code >= 0)
		{
			shell_data->status = valid_var_code;
			idx++;
			free_tab((void **)split);
			continue ;
		}
		if (split[1])
			ft_setenv(&shell_data->envp, split[0], args[idx] + ft_strlen(split[0])
				+ 1, true);
		else if (ft_strchr(args[idx], '='))
			ft_setenv(&shell_data->envp, split[0], "", true);
		export_var(&shell_data->exported, split[0]);
		free_tab((void **)split);
		idx++;
	}
}
