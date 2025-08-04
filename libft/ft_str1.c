/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:11:29 by aheisch           #+#    #+#             */
/*   Updated: 2025/03/03 16:11:29 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
		return (0);
	while (--n)
	{
		if (*s1 != *s2)
			break ;
		if (!*s1 || !*s2)
			break ;
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	idx_b;
	size_t	idx_l;

	if (!*little)
		return ((char *)big);
	idx_b = 0;
	while (idx_b < len && big[idx_b])
	{
		idx_l = 0;
		while (idx_b + idx_l < len && little[idx_l] && big[idx_b + idx_l]
			&& little[idx_l] == big[idx_b + idx_l])
			idx_l++;
		if (little[idx_l] == '\0')
			return ((char *)big + idx_b);
		idx_b++;
	}
	return (NULL);
}

char	*ft_strdup(const char *s)
{
	int		len;
	int		idx;
	char	*dup;

	len = ft_strlen(s);
	dup = malloc(sizeof(char) * (len + 1));
	if (!dup)
		return (NULL);
	idx = 0;
	while (s[idx])
	{
		dup[idx] = s[idx];
		idx++;
	}
	dup[idx] = '\0';
	return (dup);
}

char	*ft_strndup(const char *src, unsigned int n)
{
	char	*dup;

	if (!src)
		return (NULL);
	dup = (char *)malloc(sizeof(char) * (n + 1));
	if (!dup)
		return (NULL);
	ft_strncpy(dup, src, n);
	dup[n] = '\0';
	return (dup);
}
