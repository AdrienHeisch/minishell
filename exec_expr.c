/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:07:14 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/28 20:50:40 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

void	exec_expr(t_expr *expr, t_shell_data *shell_data)
{
	if (!expr)
		return ;
	if (expr->type == EX_CMD)
		exec_cmd(expr->data.cmd, shell_data);
	else if (expr->type == EX_PIPE)
		shell_data->status = exec_pipe(expr->data.pipe, shell_data);
	else if (expr->type == EX_LIST)
		exec_expr_list(expr->data.expr_list, shell_data);
	else
		exit(MS_UNREACHABLE);
}
