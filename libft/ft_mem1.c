/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mem1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:17:58 by aheisch           #+#    #+#             */
/*   Updated: 2025/03/03 16:17:58 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*bytes;

	bytes = (const unsigned char *)s;
	while (n--)
	{
		if (*bytes == (unsigned char)c)
			return ((void *)bytes);
		bytes++;
	}
	return (NULL);
}

void	*ft_memrchr(const void *s, int c, size_t n)
{
	const char	*bytes;
	char		*ret;

	bytes = (const char *)s;
	ret = NULL;
	while (n--)
	{
		if (*bytes == c)
			ret = (void *)bytes;
		bytes++;
	}
	return (ret);
}
