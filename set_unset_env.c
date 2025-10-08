/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_unset_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 23:58:54 by aheisch           #+#    #+#             */
/*   Updated: 2025/10/08 23:58:54 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <unistd.h>
#include <stdlib.h>

static t_err	add_to_env(char ***envp, const char *name, const char *value)
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
		return (ERR_SYSTEM);
	ft_memcpy(new, old, len * sizeof(char *));
	name_suffix = ft_strjoin(name, "=");
	if (!name_suffix)
		return (ERR_SYSTEM);
	new[len] = ft_strjoin(name_suffix, value);
	if (!new[len])
		return (ERR_SYSTEM);
	new[len + 1] = NULL;
	free(name_suffix);
	free(old);
	*envp = new;
	return (ERR_OK);
}

static t_err	do_all_vars(char ***envp, char *name_suffix,
	const char *value, size_t len)
{
	char	*name_new;
	size_t	idx;

	idx = 0;
	while ((*envp)[idx])
	{
		if (ft_strncmp((*envp)[idx], name_suffix, len) == 0)
		{
			name_new = ft_strjoin(name_suffix, value);
			if (!name_new)
				return (free(name_suffix), ERR_SYSTEM);
			free((*envp)[idx]);
			(*envp)[idx] = name_new;
			break ;
		}
		idx++;
	}
	return (ERR_OK);
}

static void	unsetenv_loop(size_t len, char *name_suffix, char **new, char **old)
{
	size_t	idx;
	size_t	n_skipped;

	idx = 0;
	n_skipped = 0;
	while (idx + n_skipped < len)
	{
		if (ft_strncmp(old[idx + n_skipped], name_suffix,
				ft_strlen(name_suffix)) == 0)
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

/// Returns ERR_OK or ERR_SYSTEM
t_err	ft_unsetenv(char ***envp, const char *name)
{
	char	**old;
	char	**new;
	char	*name_suffix;
	size_t	len;

	if (!ft_getenv(*envp, name))
		return (ERR_OK);
	old = *envp;
	len = 0;
	while (old[len])
		len++;
	name_suffix = ft_strjoin(name, "=");
	if (!name_suffix)
		return (ERR_SYSTEM);
	new = malloc(len * sizeof(char *));
	if (!new)
		return (ERR_SYSTEM);
	unsetenv_loop(len, name_suffix, new, old);
	free(name_suffix);
	free(old);
	*envp = new;
	return (ERR_OK);
}

/// Returns ERR_OK or ERR_SYSTEM
t_err	ft_setenv(char ***envp, const char *name, const char *value,
		int overwrite)
{
	char	*name_suffix;
	t_err	err;
	size_t	len;

	if (!value)
		ft_unsetenv(envp, name);
	if (!ft_getenv(*envp, name))
	{
		add_to_env(envp, name, value);
		return (ERR_OK);
	}
	if (!overwrite)
		return (ERR_OK);
	name_suffix = ft_strjoin(name, "=");
	if (!name_suffix)
		return (ERR_SYSTEM);
	len = ft_strlen(name_suffix);
	err = do_all_vars(envp, name_suffix, value, len);
	if (err == ERR_SYSTEM)
		return (ERR_SYSTEM);
	free(name_suffix);
	return (ERR_OK);
}
