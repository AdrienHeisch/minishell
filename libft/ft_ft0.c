/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ft0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 18:45:03 by aheisch           #+#    #+#             */
/*   Updated: 2025/03/03 18:45:03 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	cpy_len;

	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	s += start;
	cpy_len = ft_strlen(s);
	if (cpy_len > len)
		cpy_len = len;
	sub = (char *)malloc(sizeof(char) * (cpy_len + 1));
	if (!sub)
		return (NULL);
	ft_memcpy(sub, s, cpy_len);
	sub[cpy_len] = '\0';
	return (sub);
}

static int	ft_isoneof(char c, const char *cs)
{
	while (*cs)
	{
		if (c == *cs)
			return (1);
		cs++;
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	char		*trim;
	const char	*cpy_start;
	size_t		cpy_len;

	while (*s1 && ft_isoneof(*s1, set))
		s1++;
	cpy_start = s1;
	while (*s1)
		s1++;
	s1--;
	while (s1 > cpy_start && ft_isoneof(*s1, set))
		s1--;
	cpy_len = s1 - cpy_start + 1;
	trim = (char *)malloc(sizeof(char) * (cpy_len + 1));
	if (!trim)
		return (NULL);
	ft_memcpy(trim, cpy_start, cpy_len);
	trim[cpy_len] = '\0';
	return (trim);
}

char	*ft_strmapi(const char *s, char (*f)(unsigned int, char))
{
	char	*str;
	size_t	idx;

	str = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	idx = 0;
	while (s[idx])
	{
		str[idx] = f(idx, s[idx]);
		idx++;
	}
	str[idx] = '\0';
	return (str);
}

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	size_t	idx;

	idx = 0;
	while (s[idx])
	{
		f(idx, &s[idx]);
		idx++;
	}
}
