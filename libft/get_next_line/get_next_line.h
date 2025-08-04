/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 23:05:19 by aheisch           #+#    #+#             */
/*   Updated: 2025/06/04 17:29:47 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include "../libft.h"
# include <stddef.h>

char					*ft_get_next_line(int fd);

typedef struct s_gnl_list
{
	t_string			str;
	int					fd;
	struct s_gnl_list	*next;
}						t_gnl_list;

#endif // !GET_NEXT_LINE_H
