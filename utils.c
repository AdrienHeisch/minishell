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

char	*ft_getenv(char **envp, char *name)
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

void	ft_setenv(char ***envp, const char *name, const char *value, int overwrite)
{
	char	**old;
	char	**new;
	char	*name_suffix;
	size_t	len;

	(void)overwrite;
	old = *envp;
	len = 0;
	while (old[len])
		len++;
	new = malloc(( len + 2 ) * sizeof(char *));
	ft_memcpy(new, old, len * sizeof(char *));
	name_suffix = ft_strjoin(name, "=");
	new[len] = ft_strjoin(name_suffix, value);
	new[len + 1] = NULL;
	free(name_suffix);
	free(old);
	*envp = new;
}
