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

static void	print_redir(t_redir_data *redir)
{
	if (redir->type == REDIR_IN)
		printf(" < ");
	else if (redir->type == REDIR_HEREDOC)
		printf(" << ");
	else if (redir->type == REDIR_OUT)
		printf(" > ");
	else if (redir->type == REDIR_APPEND)
		printf(" >> ");
	else
		exit(ERR_UNREACHABLE);
	printf("%s", redir->file_name.content);
}

static void	print_expr_rec(t_expr *expr)
{
	t_list	*lst;

	if (!expr)
	{
		printf("(null)");
		return ;
	}
	printf("[");
	if (expr->type == EX_CMD)
	{
		lst = expr->u_data.cmd.args;
		while (lst)
		{
			printf("%s", ((t_string *)lst->content)->content);
			lst = lst->next;
			if (lst)
				printf(" ");
		}
	}
	else if (expr->type == EX_BINOP)
	{
		print_expr_rec(expr->u_data.binop.left);
		if (expr->u_data.binop.op == OP_PIPE)
			printf(" | ");
		else if (expr->u_data.binop.op == OP_AND)
			printf(" && ");
		else if (expr->u_data.binop.op == OP_OR)
			printf(" || ");
		else
			exit(ERR_UNREACHABLE);
		print_expr_rec(expr->u_data.binop.right);
	}
	else if (expr->type == EX_PARENTHESES)
	{
		printf("(");
		print_expr_rec(expr->u_data.paren.inner);
		printf(")");
	}
	else
		exit(ERR_UNREACHABLE);
	ft_lstiter(expr->redirs, (void (*)(void *))print_redir);
	printf("]");
}

void	print_expr(t_expr *expr)
{
	print_expr_rec(expr);
	printf("\n");
}
