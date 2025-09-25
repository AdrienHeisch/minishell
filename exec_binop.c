/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_binop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 21:20:11 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/28 21:20:11 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// Returns ERR_OK or ERR_SYSTEM
t_err	exec_binop(t_binop binop, t_shell_data *shell_data)
{
	if (binop.op == OP_PIPE)
		return (exec_pipe(binop, shell_data));
	else
	{
		if (exec_expr(binop.left, shell_data))
			return (ERR_SYSTEM);
		if ((binop.op == OP_AND && shell_data->status == 0)
			|| (binop.op == OP_OR && shell_data->status != 0))
		{
			if (exec_expr(binop.right, shell_data))
				return (ERR_SYSTEM);
		}
		return (ERR_OK);
	}
}
