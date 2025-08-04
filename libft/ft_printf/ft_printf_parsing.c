/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_parsing.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:45:12 by aheisch           #+#    #+#             */
/*   Updated: 2025/05/14 11:45:12 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"
#include <stddef.h>
#include <stdlib.h>

static size_t	parse_number(const char **s, t_format *format)
{
	t_string	digits;
	char		*tmp;
	size_t		n;

	digits = ft_string_new();
	if (!digits.content)
		return (format->is_valid = false, 0);
	while (ft_isdigit(**s))
	{
		if (!ft_string_ncat(&digits, (*s)++, 1))
		{
			format->is_valid = false;
			return (0);
		}
	}
	format->length += digits.length;
	tmp = ft_string_into(&digits);
	if (!tmp)
		return (format->is_valid = false, 0);
	n = ft_atoul(tmp);
	free(tmp);
	return (n);
}

t_format	parse(const char **s)
{
	t_format	format;

	format = ft_format_new();
	format.is_valid = true;
	while (**s == '#' || **s == ' ' || **s == '+' || **s == '-' || **s == '0')
	{
		format.flags[(long)**s] = 1;
		(*s)++;
		format.length++;
	}
	format.width = parse_number(s, &format);
	if (!format.is_valid)
		return (format);
	if (**s == '.')
	{
		format.has_precision = true;
		format.flags[(long)**s] = 1;
		(*s)++;
		format.length++;
	}
	if (format.has_precision)
		format.precision = parse_number(s, &format);
	if (!format.is_valid)
		return (format);
	return (format);
}
