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

static void print_redir(t_redir_data *redir)
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
		exit(MS_UNREACHABLE);
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
		lst = expr->data.cmd.args;
		while (lst)
		{
			printf("%s", ft_string_get(lst->content));
			lst = lst->next;
			if (lst)
				printf(" ");
		}
		ft_lstiter(expr->data.cmd.redirs, (void (*)(void *))print_redir);
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
	else if (expr->type == EX_PARENTHESES)
	{
		printf("(");
		print_expr_rec(expr->data.paren.inner);
		printf(")");
		ft_lstiter(expr->data.paren.redirs, (void (*)(void *))print_redir);
	}
	else
		exit(MS_UNREACHABLE);
	printf("]");
}

void	print_expr(t_expr *expr)
{
	print_expr_rec(expr);
	printf("\n");
}
