/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 11:25:04 by aheisch           #+#    #+#             */
/*   Updated: 2025/05/16 11:25:04 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"
#include <stdlib.h>

t_string	ft_untoa_precision(unsigned long long n, const char *base,
		size_t precision)
{
	t_string	str;
	char		*tmp;
	size_t		tmp_len;

	str = ft_string_new();
	if (!str.content)
		return (str);
	tmp = ft_untoa(n, base);
	if (!tmp)
		return (ft_string_destroy(&str));
	tmp_len = ft_strlen(tmp);
	if (precision > tmp_len)
		precision -= tmp_len;
	else
		precision = 0;
	while (str.length < precision)
	{
		if (!ft_string_cat(&str, "0"))
			return (free(tmp), ft_string_destroy(&str));
	}
	if (!ft_string_cat(&str, tmp))
		return (free(tmp), ft_string_destroy(&str));
	return (free(tmp), str);
}

t_string	ft_untoa_format(const unsigned int n, const char *base,
		const t_format *format)
{
	if (format->has_precision)
		return (ft_untoa_precision(n, base, format->precision));
	else
		return (ft_untoa_precision(n, base, 0));
}

t_format	ft_format_new(void)
{
	t_format	format;

	format.is_valid = false;
	format.length = 0;
	format.width = 0;
	format.has_precision = false;
	format.precision = 0;
	ft_bzero(format.flags, 128);
	return (format);
}

t_convert_out	ft_convert_out_new(void)
{
	t_convert_out	out;

	out.prefix = (t_string){.capacity = 0, .content = NULL, .length = 0};
	out.body = (t_string){.capacity = 0, .content = NULL, .length = 0};
	return (out);
}

void	ft_convert_out_destroy(t_convert_out *out)
{
	ft_string_destroy(&out->prefix);
	ft_string_destroy(&out->body);
}
