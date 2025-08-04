/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_char.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:52:58 by aheisch           #+#    #+#             */
/*   Updated: 2025/05/14 12:52:58 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

t_convert_out	convert_char(const char c)
{
	t_convert_out	out;

	out = ft_convert_out_new();
	out.body = ft_string_new();
	if (!out.body.content)
		return (out);
	ft_string_ncat(&out.body, (char *)&c, 1);
	return (out);
}
