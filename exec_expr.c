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

/// Returns ERR_OK or ERR_SYSTEM
t_err	exec_expr(t_expr *expr, t_shell_data *shell_data)
{
	if (!expr)
		exit(ERR_LOGIC);
	if (expr->type == EX_CMD)
		return (exec_cmd(expr, shell_data));
	else if (expr->type == EX_BINOP)
		return (exec_binop(expr->u_data.binop, shell_data));
	else if (expr->type == EX_PARENTHESES)
		return (exec_parentheses(expr, shell_data));
	else
		exit(ERR_UNREACHABLE);
}
