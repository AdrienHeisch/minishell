/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 23:58:37 by aheisch           #+#    #+#             */
/*   Updated: 2025/10/08 23:58:37 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static t_err	increment_shlvl(char ***dup)
{
	char	*shlvl;
	char	*new_shlvl;

	shlvl = ft_getenv(*dup, "SHLVL");
	if (shlvl)
	{
		new_shlvl = ft_itoa(ft_atoi(shlvl) + 1);
		if (!new_shlvl || ft_setenv(dup, "SHLVL", new_shlvl, true))
			return (free(new_shlvl), ERR_SYSTEM);
		free(new_shlvl);
	}
	else if (ft_setenv(dup, "SHLVL", "0", true))
		return (ERR_SYSTEM);
	return (ERR_OK);
}

/// Will return null and errno will be set on error
char	**dup_env(char **envp)
{
	char	**dup;
	size_t	len;
	size_t	idx;
	char	*cwd;

	len = 0;
	while (envp[len])
		len++;
	dup = ft_calloc(len + 1, sizeof(char *));
	if (!dup)
		return (NULL);
	idx = 0;
	while (envp[idx])
	{
		dup[idx] = ft_strdup(envp[idx]);
		if (!dup[idx++])
			return (free_tab((void ***)&dup), NULL);
	}
	cwd = getcwd(NULL, 0);
	if (cwd && ft_setenv(&dup, "PWD", cwd, true))
		return (free_tab((void ***)&dup), free(cwd), NULL);
	free(cwd);
	if (increment_shlvl(&dup))
		return (free_tab((void ***)&dup), NULL);
	return (dup);
}

/// Will return null and errno will be set on error
char	**make_export_list(char **envp)
{
	char	**list;
	size_t	len;
	size_t	idx;
	char	**split;

	len = 0;
	while (envp[len])
		len++;
	list = ft_calloc(len + 1, sizeof(char *));
	if (!list)
		return (NULL);
	idx = 0;
	while (envp[idx])
	{
		split = ft_split(envp[idx], '=');
		if (!split)
			return (NULL);
		if (!split[0])
			continue ;
		list[idx] = ft_strdup(split[0]);
		if (!list[idx++])
			return (NULL);
		free_tab((void ***)&split);
	}
	return (list);
}

void	free_shell_data(t_shell_data *shell_data)
{
	free_tab((void ***)&shell_data->envp);
	free_tab((void ***)&shell_data->exported);
	free_expr(shell_data->ast_root);
}

t_err	argument_mode(char **argv, t_shell_data *data)
{
	t_string	str;
	char		**tab;
	size_t		idx;

	tab = ft_split(argv[2], '\n');
	if (!tab)
		return (ERR_SYSTEM);
	idx = 0;
	while (tab[idx])
	{
		str = ft_string_from(tab[idx++]);
		if (parse_and_exec(&str, data))
			break ;
		ft_string_destroy(&str);
	}
	return (free(tab), ft_string_destroy(&str), ERR_OK);
}
