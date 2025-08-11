/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 21:18:18 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/11 21:18:18 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

bool	is_builtin(t_string *name)
{
	static const char *list[] = {"echo", "env", "export", 0};
	size_t	idx;

	idx = 0;
	while (list[idx])
	{
		if (ft_strncmp(list[idx], name->content, name->length) == 0)
			return (true);
		idx++;
	}
	return (false);
}
