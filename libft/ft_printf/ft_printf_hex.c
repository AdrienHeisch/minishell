/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_hex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:41:23 by aheisch           #+#    #+#             */
/*   Updated: 2025/05/14 14:41:23 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

t_convert_out	convert_hex(const unsigned int n, const t_format *format,
		bool uppercase)
{
	t_convert_out	out;

	out = ft_convert_out_new();
	if (format->flags['#'] && n > 0)
	{
		out.prefix = ft_string_new();
		if (!out.prefix.content)
			return (out);
	}
	if (uppercase)
	{
		if (out.prefix.content)
			ft_string_cat(&out.prefix, "0X");
		out.body = ft_untoa_format(n, "0123456789ABCDEF", format);
	}
	else
	{
		if (out.prefix.content)
			ft_string_cat(&out.prefix, "0x");
		out.body = ft_untoa_format(n, "0123456789abcdef", format);
	}
	return (out);
}
