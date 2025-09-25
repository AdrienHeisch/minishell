/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 12:57:27 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/06 12:58:25 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

void	print_token(t_token *token)
{
	if (token->type == TK_ARG)
		printf("ARG: |%s|", token->u_data.arg.string.content);
	else if (token->type == TK_PIPE)
		printf("|");
	else if (token->type == TK_OR)
		printf("||");
	else if (token->type == TK_AND)
		printf("&&");
	else if (token->type == TK_REDIR)
	{
		if (token->u_data.redir.type == REDIR_IN)
			printf("< ");
		else if (token->u_data.redir.type == REDIR_HEREDOC)
			printf("<< ");
		else if (token->u_data.redir.type == REDIR_OUT)
			printf("> ");
		else if (token->u_data.redir.type == REDIR_APPEND)
			printf(">> ");
		else
			exit(ERR_UNREACHABLE);
		printf("%s", token->u_data.redir.file_name.content);
	}
	else if (token->type == TK_PAROPEN)
		printf("(");
	else if (token->type == TK_PARCLOSE)
		printf(")");
	else if (token->type == TK_INVALID)
		printf("!!! INVALID !!!");
	else
		exit(ERR_UNREACHABLE);
	printf("\n");
}
