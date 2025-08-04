/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:46:02 by aheisch           #+#    #+#             */
/*   Updated: 2025/04/30 17:46:02 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static size_t	next_word(const char **s, char c)
{
	size_t	idx;

	idx = 0;
	while ((*s)[idx] && (*s)[idx] == c)
		idx++;
	*s += idx;
	idx = 0;
	while ((*s)[idx] && (*s)[idx] != c)
		idx++;
	return (idx);
}

static char	*next_word_cpy(const char **s, char c)
{
	char	*word;
	size_t	len;

	len = next_word(s, c);
	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	ft_strlcpy(word, *s, len + 1);
	*s += len;
	return (word);
}

static unsigned int	count_words(const char *s, char c)
{
	unsigned int	count;
	size_t			len;

	count = 0;
	while (*s)
	{
		len = next_word(&s, c);
		if (len > 0)
			count++;
		s += len;
	}
	return (count);
}

static void	free_tab(char **tab, size_t len)
{
	size_t	idx;

	idx = 0;
	while (idx < len)
	{
		free(tab[idx]);
		idx++;
	}
	free(tab);
}

char	**ft_split(const char *s, char c)
{
	char			**split;
	char			*new_word;
	unsigned int	n_words;
	size_t			idx;

	n_words = count_words(s, c);
	split = (char **)malloc(sizeof(char *) * (n_words + 1));
	if (!split)
		return (NULL);
	idx = 0;
	while (idx < n_words)
	{
		new_word = next_word_cpy(&s, c);
		if (!new_word)
		{
			free_tab(split, idx);
			return (NULL);
		}
		split[idx] = new_word;
		idx++;
	}
	split[n_words] = NULL;
	return (split);
}
