/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:47:33 by aheisch           #+#    #+#             */
/*   Updated: 2025/03/14 15:47:33 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

#define BUFFER_SIZE 1024

t_string	ft_read_file(int fd)
{
	t_string	str;
	char		buf[BUFFER_SIZE];
	int			n_read;

	str = ft_string_new();
	if (fd == -1)
		return (ft_string_destroy(&str));
	if (!str.content)
		str = ft_string_new();
	while (1)
	{
		n_read = read(fd, buf, BUFFER_SIZE);
		if (n_read == -1)
			return (ft_string_destroy(&str));
		if (n_read == 0)
			break ;
		ft_string_ncat(&str, buf, n_read);
	}
	return (str);
}
