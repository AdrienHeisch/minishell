/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:11:34 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/13 19:11:34 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

static void	remove_export_loop(size_t len, char *name, char **new, char **old)
{
	size_t	idx;
	size_t	n_skipped;

	idx = 0;
	n_skipped = 0;
	while (idx + n_skipped < len)
	{
		if (ft_strncmp(old[idx + n_skipped], name,
				ft_strlen(old[idx + n_skipped])) == 0)
		{
			free(old[idx + n_skipped]);
			n_skipped++;
			continue ;
		}
		new[idx] = old[idx + n_skipped];
		idx++;
	}
	new[idx] = NULL;
}

static bool	contains(char **exported, char *name)
{
	size_t	idx;

	idx = 0;
	while (exported[idx])
	{
		if (exported[idx] == name)
			return (true);
		idx++;
	}
	return (false);
}

static t_err	remove_export(char ***exported, char *name)
{
	char	**old;
	char	**new;
	size_t	len;

	if (!contains(*exported, name))
		return (ERR_OK);
	old = *exported;
	len = 0;
	while (old[len])
		len++;
	new = malloc(len * sizeof(char *));
	if (!new)
		return (ERR_SYSTEM);
	remove_export_loop(len, name, new, old);
	free(old);
	*exported = new;
	return (ERR_OK);
}

t_err	builtin_unset(t_exec_info *cmd, t_shell_data *shell_data)
{
	int		flags;
	size_t	idx;

	idx = 1;
	if (find_options(&flags, cmd->args, &idx, ""))
		return (ERR_SYNTAX_ERROR);
	while (cmd->args[idx])
	{
		if (ft_unsetenv(&shell_data->envp, cmd->args[idx]))
			return (ERR_SYSTEM);
		if (remove_export(&shell_data->exported, cmd->args[idx]))
			return (ERR_SYSTEM);
		idx++;
	}
	return (ERR_OK);
}
