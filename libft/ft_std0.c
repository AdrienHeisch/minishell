/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_std0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:34:56 by aheisch           #+#    #+#             */
/*   Updated: 2025/03/03 16:34:56 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

int	ft_atoi(const char *nptr)
{
	int	n;
	int	is_neg;

	is_neg = 0;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == ' ')
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			is_neg = 1;
		nptr++;
	}
	n = 0;
	while (ft_isdigit(*nptr))
	{
		n *= 10;
		n += *nptr - '0';
		nptr++;
	}
	if (is_neg)
		n *= -1;
	return (n);
}

long	ft_atol(const char *nptr)
{
	long	n;
	int		is_neg;

	is_neg = 0;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == ' ')
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			is_neg = 1;
		nptr++;
	}
	n = 0;
	while (ft_isdigit(*nptr))
	{
		n *= 10;
		n += *nptr - '0';
		nptr++;
	}
	if (is_neg)
		n *= -1;
	return (n);
}

unsigned long	ft_atoul(const char *nptr)
{
	unsigned long	n;

	while ((*nptr >= 9 && *nptr <= 13) || *nptr == ' ')
		nptr++;
	n = 0;
	while (ft_isdigit(*nptr))
	{
		n *= 10;
		n += *nptr - '0';
		nptr++;
	}
	return (n);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	len;

	len = nmemb * size;
	if (size != 0 && len / size != nmemb)
		return (NULL);
	ptr = malloc(len);
	if (ptr)
		ft_bzero(ptr, len);
	return (ptr);
}
