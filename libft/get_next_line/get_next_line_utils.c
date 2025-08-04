/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 23:06:08 by aheisch           #+#    #+#             */
/*   Updated: 2025/06/04 17:29:48 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	find_char(char *str, unsigned int n, char c)
{
	int	idx;

	idx = 0;
	while (n-- && str[idx] != '\0')
	{
		if (str[idx] == c)
			return (idx);
		idx++;
	}
	return (-1);
}
