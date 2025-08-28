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

static void	print_expr_rec(t_expr *expr)
{
	t_list	*lst;

	if (!expr)
	{
		printf("(null)");
		return ;
	}
	printf("(");
	if (expr->type == EX_CMD)
	{
		lst = expr->data.cmd.args;
		while (lst)
		{
			printf("%s", ft_string_get(lst->content));
			lst = lst->next;
			if (lst)
				printf(" ");
		}
		// if (expr->data.cmd.file_out.content)
		// 	printf("> %s ", expr->data.cmd.file_out.content);
	}
	else if (expr->type == EX_BINOP)
	{
		print_expr_rec(expr->data.binop.left);
		if (expr->data.binop.op == OP_PIPE)
			printf(" | ");
		else if (expr->data.binop.op == OP_AND)
			printf(" && ");
		else if (expr->data.binop.op == OP_OR)
			printf(" || ");
		else
			exit(MS_UNREACHABLE);
		print_expr_rec(expr->data.binop.right);
	}
	else
		exit(MS_UNREACHABLE);
	printf(")");
}

void	print_expr(t_expr *expr)
{
	print_expr_rec(expr);
	printf("\n");
}
