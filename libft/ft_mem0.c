/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mem.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 15:16:51 by aheisch           #+#    #+#             */
/*   Updated: 2025/03/03 15:16:51 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*bytes;

	bytes = (unsigned char *)s;
	while (n--)
		*bytes++ = (unsigned char)c;
	return (s);
}

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*m1;
	const unsigned char	*m2;

	m1 = (const unsigned char *)s1;
	m2 = (const unsigned char *)s2;
	if (n == 0)
		return (0);
	while (--n)
	{
		if (*m1 != *m2)
			break ;
		m1++;
		m2++;
	}
	return (*m1 - *m2);
}

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*dest_bytes;
	const unsigned char	*src_bytes;

	if (dest == src)
		return (dest);
	dest_bytes = (unsigned char *)dest;
	src_bytes = (const unsigned char *)src;
	while (n--)
		*dest_bytes++ = *src_bytes++;
	return (dest);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*dest_bytes;
	const unsigned char	*src_bytes;

	dest_bytes = (unsigned char *)dest;
	src_bytes = (const unsigned char *)src;
	if (dest == src)
		return (dest);
	if (dest < src)
	{
		while (n--)
			*dest_bytes++ = *src_bytes++;
	}
	else
	{
		while (n--)
			dest_bytes[n] = src_bytes[n];
	}
	return (dest);
}
