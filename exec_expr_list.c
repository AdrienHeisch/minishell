/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expr_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:46:48 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/28 17:46:48 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_expr_list(t_expr_list list, t_shell_data *shell_data)
{
	exec(list.left, shell_data);
	if ((list.is_or && shell_data->status != 0) || (!list.is_or
			&& shell_data->status == 0))
		exec(list.right, shell_data);
}
