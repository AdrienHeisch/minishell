/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_expr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:53:27 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/06 15:53:27 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

void	free_expr(t_expr *expr)
{
	if (expr->type == EX_CMD)
	{
		ft_lstclear(&expr->data.cmd.args, lstclear_string);
		// TODO free_redir
		ft_lstclear(&expr->data.cmd.redirs, no_op);
	}
	else if (expr->type == EX_PIPE)
	{
		free_expr(expr->data.pipe.left);
		free_expr(expr->data.pipe.right);
	}
	free(expr);
}
