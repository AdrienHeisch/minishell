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
#include <errno.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

void	no_op(void *p)
{
	(void)p;
}

bool	is_str_all(char *s, int f(int))
{
	while (*s)
	{
		if (!f(*s))
			return (false);
		s++;
	}
	return (true);
}

bool	is_whitespace(char c)
{
	static const char	*list = " \t\n";

	if (!c)
		return (false);
	return (ft_strchr(list, c) != NULL);
}

bool	is_only_whitespace(t_string *str)
{
	size_t	idx;

	idx = 0;
	while (idx < str->length)
	{
		if (!is_whitespace(str->content[idx]))
			return (false);
		idx++;
	}
	return (true);
}

size_t	size_t_max(size_t a, size_t b)
{
	if (a > b)
		return (a);
	else
		return (b);
}
