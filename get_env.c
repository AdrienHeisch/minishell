/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 23:58:58 by aheisch           #+#    #+#             */
/*   Updated: 2025/10/08 23:58:58 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h" 

char	*ft_getenv(char **envp, const char *name)
{
	int		idx;
	size_t	len;

	len = ft_strlen(name);
	idx = 0;
	while (envp[idx])
	{
		if (ft_strncmp(envp[idx], name, len) == 0 && envp[idx][len] == '=')
			return (envp[idx] + len + 1);
		idx++;
	}
	return (NULL);
}
