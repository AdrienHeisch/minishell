/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 20:01:55 by aheisch           #+#    #+#             */
/*   Updated: 2025/04/27 20:01:55 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_string	ft_string_destroy(t_string *str)
{
	if (str->content)
		free(str->content);
	str->content = NULL;
	str->length = 0;
	str->capacity = 0;
	return (*str);
}

void	*ft_string_delete(t_string **str)
{
	ft_string_destroy(*str);
	free(*str);
	*str = NULL;
	return (NULL);
}

char	*ft_string_into(t_string *str)
{
	char	*out;

	out = ft_string_get(str);
	ft_string_destroy(str);
	return (out);
}
