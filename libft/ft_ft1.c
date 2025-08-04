/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ft1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 22:21:52 by aheisch           #+#    #+#             */
/*   Updated: 2025/03/03 22:21:52 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static char	*ft_ntoa_putnbr(char *s, unsigned long long n, const char *base,
		size_t base_len)
{
	if (n >= base_len)
		s = ft_ntoa_putnbr(s, n / base_len, base, base_len);
	*s = base[n % base_len];
	return (s + 1);
}

char	*ft_ntoa(long long n, const char *base)
{
	unsigned long long	un;
	char				buf[66];
	char				*str;
	size_t				len;

	str = buf;
	if (n < 0)
	{
		*str++ = '-';
		un = -n;
	}
	else
	{
		un = n;
	}
	str = ft_ntoa_putnbr(str, un, base, ft_strlen(base));
	*str = '\0';
	len = ft_strlen(buf);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	ft_memcpy(str, buf, len);
	str[len] = '\0';
	return (str);
}

char	*ft_untoa(unsigned long long n, const char *base)
{
	char	buf[65];
	char	*str;
	size_t	len;

	str = buf;
	str = ft_ntoa_putnbr(str, n, base, ft_strlen(base));
	*str = '\0';
	len = ft_strlen(buf);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	ft_memcpy(str, buf, len);
	str[len] = '\0';
	return (str);
}

char	*ft_itoa(int n)
{
	return (ft_ntoa(n, "0123456789"));
}
