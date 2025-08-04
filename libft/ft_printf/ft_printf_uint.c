/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_uint.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:38:24 by aheisch           #+#    #+#             */
/*   Updated: 2025/05/14 14:38:24 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

t_convert_out	convert_uint(const unsigned int n, const t_format *format)
{
	t_convert_out	out;

	out = ft_convert_out_new();
	out.body = ft_untoa_format(n, "0123456789", format);
	return (out);
}
