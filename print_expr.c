/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_expr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 12:59:18 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/06 12:59:18 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

void	print_expr(t_expr *expr)
{
	t_list	*lst;

	if (expr->type == EX_CMD)
	{
		printf("CMD: ");
		lst = expr->data.cmd.args;
		while (lst)
		{
			printf("%s ", ft_string_get(lst->content));
			lst = lst->next;
		}
		// if (expr->data.cmd.file_out.content)
		// 	printf("> %s ", expr->data.cmd.file_out.content);
	}
	else if (expr->type == EX_PIPE)
	{
		printf("PIPE_START\n");
		print_expr(expr->data.pipe.left);
		print_expr(expr->data.pipe.right);
		printf("PIPE_END");
	}
	else
		exit(MS_UNREACHABLE);
	printf("\n");
}
