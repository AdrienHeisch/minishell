/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:28:22 by aheisch           #+#    #+#             */
/*   Updated: 2025/05/14 14:28:22 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

static int	make_prefix(t_convert_out *out, const char *prefix)
{
	out->prefix = ft_string_new();
	if (!out->prefix.content)
		return (1);
	ft_string_cat(&out->prefix, prefix);
	return (0);
}

t_convert_out	convert_int(const int n, const t_format *format)
{
	t_convert_out	out;
	unsigned int	un;

	out = ft_convert_out_new();
	un = n;
	if (n >= 0 && format->flags['+'])
	{
		if (make_prefix(&out, "+"))
			return (out);
	}
	else if (n >= 0 && format->flags[' '])
	{
		if (make_prefix(&out, " "))
			return (out);
	}
	if (n < 0)
	{
		un = -n;
		if (make_prefix(&out, "-"))
			return (out);
	}
	out.body = ft_untoa_format(un, "0123456789", format);
	return (out);
}
