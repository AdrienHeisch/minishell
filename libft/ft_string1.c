/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 22:34:34 by aheisch           #+#    #+#             */
/*   Updated: 2025/03/09 22:34:34 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

void	ft_string_set(t_string *str, const char *content)
{
	size_t	len;

	len = ft_strlen(content);
	ft_strncpy(str->content, content, len);
	str->length = len;
}

t_string	*ft_string_ncat(t_string *str, const char *cat, size_t n)
{
	size_t	old_len;
	size_t	old_cap;
	char	*new_content;

	old_len = str->length;
	str->length += n;
	old_cap = str->capacity;
	while (str->capacity < str->length)
		str->capacity *= 2;
	if (str->capacity > old_cap)
	{
		new_content = (char *)malloc(sizeof(char) * str->capacity);
		if (!new_content)
			return (NULL);
		ft_memcpy(new_content, str->content, old_len);
		ft_bzero(new_content + old_len, str->capacity - old_len);
		free(str->content);
		str->content = new_content;
	}
	while (n--)
		*(str->content + old_len++) = *cat++;
	return (str);
}

t_string	*ft_string_cat(t_string *str, const char *cat)
{
	return (ft_string_ncat(str, cat, ft_strlen(cat)));
}

t_string	*ft_string_cat_free(t_string *str, char *cat)
{
	ft_string_cat(str, cat);
	free(cat);
	return (str);
}
