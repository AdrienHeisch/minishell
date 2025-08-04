/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_string.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:04:17 by aheisch           #+#    #+#             */
/*   Updated: 2025/05/14 15:04:17 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

t_convert_out	convert_string(const char *s, const t_format *format)
{
	t_convert_out	out;

	out = ft_convert_out_new();
	out.body = ft_string_new();
	if (!out.body.content)
		return (out);
	if (s == NULL)
		ft_string_cat(&out.body, "(null)");
	else
		ft_string_cat(&out.body, s);
	if (format->has_precision && format->precision < out.body.length)
	{
		out.body.length = format->precision;
	}
	return (out);
}
