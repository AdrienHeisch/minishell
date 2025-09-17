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

static void	free_redir(t_redir_data *redir)
{
	ft_string_destroy(&redir->file_name);
	free(redir);
}

void	free_expr(t_expr *expr)
{
	if (!expr)
		return ;
	if (expr->type == EX_CMD)
		ft_lstclear(&expr->data.cmd.args, lstclear_string);
	else if (expr->type == EX_BINOP)
	{
		free_expr(expr->data.binop.left);
		free_expr(expr->data.binop.right);
	}
	else if (expr->type == EX_PARENTHESES)
		free_expr(expr->data.paren.inner);
	else
		exit(MS_UNREACHABLE);
	ft_lstclear(&expr->redirs, (void (*)(void *))free_redir);
	free(expr);
}
