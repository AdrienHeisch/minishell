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

void	exec_binop(t_binop binop, t_shell_data *shell_data)
{
	if (binop.op == OP_PIPE)
		exec_pipe(binop, shell_data);
	else
	{
		exec_expr(binop.left, shell_data);
		if ((binop.op == OP_AND && shell_data->status == 0)
			|| (binop.op == OP_OR && shell_data->status != 0))
			exec_expr(binop.right, shell_data);
	}
}
