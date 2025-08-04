/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 22:13:16 by aheisch           #+#    #+#             */
/*   Updated: 2025/03/08 22:13:16 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include "ft_printf_int.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

t_convert_out	convert_percent(void)
{
	t_convert_out	out;

	out = ft_convert_out_new();
	out.body = ft_string_new();
	if (!out.body.content)
		return (out);
	ft_string_cat(&out.body, "%");
	return (out);
}

t_convert_out	convert(const char specifier, const t_format *format,
		va_list args)
{
	t_convert_out	out;

	out = ft_convert_out_new();
	if (!format->is_valid)
		return (out);
	if (specifier == 'c')
		return (convert_char(va_arg(args, int)));
	else if (specifier == 'i' || specifier == 'd')
		return (convert_int(va_arg(args, int), format));
	else if (specifier == 'u')
		return (convert_uint(va_arg(args, unsigned int), format));
	else if (specifier == 'x')
		return (convert_hex(va_arg(args, unsigned int), format, false));
	else if (specifier == 'X')
		return (convert_hex(va_arg(args, unsigned int), format, true));
	else if (specifier == 'p')
		return (convert_pointer(va_arg(args, void *)));
	else if (specifier == 's')
		return (convert_string(va_arg(args, char *), format));
	else if (specifier == '%')
		return (convert_percent());
	return (out);
}

int				format1(t_string *str, t_convert_out *converted,
					const t_format *format, size_t width_count);

int	format(t_string *str, const char **fmt, va_list args)
{
	t_convert_out	converted;
	size_t			width_count;
	const t_format	format = parse(fmt);

	if (!format.is_valid)
		return (1);
	converted = convert(**fmt, &format, args);
	(*fmt)++;
	if (!converted.body.content)
		return (ft_convert_out_destroy(&converted), 2);
	width_count = format.width;
	if (converted.prefix.content && width_count >= converted.prefix.length)
		width_count -= converted.prefix.length;
	if (!format.flags['-'] && !(format.flags['0'] && !format.has_precision))
	{
		while (width_count-- > converted.body.length)
			ft_string_ncat(str, " ", 1);
	}
	return (format1(str, &converted, &format, width_count));
}

int	format1(t_string *str, t_convert_out *converted, const t_format *format,
		size_t width_count)
{
	if (converted->prefix.content)
		ft_string_ncat(str, converted->prefix.content,
			converted->prefix.length);
	if (!format->flags['-'] && (format->flags['0'] && !format->has_precision))
	{
		while (width_count-- > converted->body.length)
			ft_string_ncat(str, "0", 1);
	}
	ft_string_ncat(str, converted->body.content, converted->body.length);
	if (format->flags['-'])
	{
		while (width_count-- > converted->body.length)
		{
			if ((format->flags['0'] || format->has_precision)
				&& !format->flags['-'])
				ft_string_ncat(str, "0", 1);
			else
				ft_string_ncat(str, " ", 1);
		}
	}
	return (ft_convert_out_destroy(converted), 0);
}

int	ft_printf(const char *fmt, ...)
{
	t_string	str;
	va_list		args;
	int			out;

	str = ft_string_new();
	if (!str.content)
		return (-1);
	va_start(args, fmt);
	while (*fmt)
	{
		if (*fmt == '%')
		{
			fmt++;
			if (format(&str, &fmt, args))
				return (va_end(args), ft_string_destroy(&str), 0);
		}
		else
		{
			if (!ft_string_ncat(&str, fmt, 1))
				return (va_end(args), ft_string_destroy(&str), 0);
			fmt++;
		}
	}
	return (va_end(args), out = write(1, str.content, str.length),
		ft_string_destroy(&str), out);
}
