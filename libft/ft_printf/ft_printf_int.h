/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_int.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:40:12 by aheisch           #+#    #+#             */
/*   Updated: 2025/05/16 18:17:22 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_INT_H
# define FT_PRINTF_INT_H

# include "../libft.h"
# include <stdbool.h>

typedef struct s_format
{
	bool		is_valid;
	size_t		length;
	char		flags[128];
	size_t		width;
	bool		has_precision;
	size_t		precision;
}				t_format;

typedef struct s_convert_out
{
	t_string	prefix;
	t_string	body;
}				t_convert_out;

t_format		parse(const char **s);

t_convert_out	convert_char(const char c);
t_convert_out	convert_int(const int n, const t_format *format);
t_convert_out	convert_uint(const unsigned int n, const t_format *format);
t_convert_out	convert_hex(const unsigned int n, const t_format *format,
					bool uppercase);
t_convert_out	convert_pointer(const void *p);
t_convert_out	convert_string(const char *s, const t_format *format);

t_string		ft_untoa_precision(unsigned long long n, const char *base,
					size_t precision);
t_string		ft_untoa_format(const unsigned int n, const char *base,
					const t_format *format);
t_format		ft_format_new(void);
t_convert_out	ft_convert_out_new(void);
void			ft_convert_out_destroy(t_convert_out *out);

#endif // !FT_PRINTF_INT_H
