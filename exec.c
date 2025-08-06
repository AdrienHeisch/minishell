/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:07:14 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/06 14:07:14 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec(t_expr expr, char **envp)
{
	if (expr.type == EX_CMD)
		child_last(expr.data.cmd, envp, expr.data.cmd.fd_in,
			expr.data.cmd.fd_out);
	else if (expr.type == EX_PIPE)
		exec_pipe(expr.data.pipe, envp);
}
