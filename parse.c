/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:01:13 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/11 19:59:12 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <errno.h>
#include <stdlib.h>

/// errno will only be set on error
t_expr	*parse(t_list **tokens)
{
	t_expr	*expr;
	t_expr	*new;

	errno = 0;
	expr = NULL;
	while (*tokens)
	{
		new = parse_expr(tokens, &expr);
		if (errno || !new || expr)
			return (free_expr(new), free_expr(expr), NULL);
		expr = new;
	}
	return (expr);
}
