/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 19:59:29 by aheisch           #+#    #+#             */
/*   Updated: 2025/04/27 19:59:29 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strncpy(char *dest, const char *src, int n)
{
	int	idx;

	if (!dest || !src)
		return (NULL);
	idx = 0;
	while (idx < n && src[idx])
	{
		dest[idx] = src[idx];
		idx++;
	}
	while (idx < n)
	{
		dest[idx] = '\0';
		idx++;
	}
	return (dest);
}

char	*ft_strnchr(const char *s, int c, size_t n)
{
	size_t	idx;

	idx = 0;
	while (idx++ < n)
	{
		if (*s == (char)c)
			return ((char *)s);
		if (*s == '\0')
			return (NULL);
		s++;
	}
	return (NULL);
}
