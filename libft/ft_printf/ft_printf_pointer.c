/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_pointer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:45:09 by aheisch           #+#    #+#             */
/*   Updated: 2025/05/14 14:45:09 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

t_convert_out	convert_pointer(const void *p)
{
	t_convert_out	out;
	char			*tmp;

	out = ft_convert_out_new();
	out.body = ft_string_new();
	if (!out.body.content)
		return (out);
	if (p == NULL)
	{
		ft_string_cat(&out.body, "(nil)");
		return (out);
	}
	ft_string_cat(&out.body, "0x");
	tmp = ft_untoa((size_t)p, "0123456789abcdef");
	if (!tmp)
	{
		ft_string_destroy(&out.body);
		return (out);
	}
	ft_string_cat_free(&out.body, tmp);
	return (out);
}
