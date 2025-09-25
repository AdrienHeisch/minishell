/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string0.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 17:54:14 by aheisch           #+#    #+#             */
/*   Updated: 2025/03/09 01:44:23 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

#define FT_STRING_NEW_CAPACITY 8

t_string	ft_string_new(void)
{
	t_string	str;

	str.length = 0;
	str.content = (char *)malloc(sizeof(char) * FT_STRING_NEW_CAPACITY);
	if (!str.content)
	{
		str.capacity = 0;
		return (str);
	}
	str.capacity = FT_STRING_NEW_CAPACITY;
	ft_bzero(str.content, str.capacity);
	return (str);
}

t_string	ft_string_from(char *s)
{
	t_string	str;

	str.content = s;
	if (!str.content)
	{
		str.length = 0;
		str.capacity = 0;
		return (str);
	}
	str.length = ft_strlen(s);
	str.capacity = str.length + 1;
	return (str);
}

t_string	*ft_string_new_ptr(void)
{
	t_string	*str;

	str = (t_string *)malloc(sizeof(t_string));
	if (!str)
		return (NULL);
	*str = ft_string_new();
	if (!str->content)
		return (ft_string_delete(&str));
	return (str);
}

char	*ft_string_get(t_string *str)
{
	return (ft_strndup(str->content, str->length));
}
