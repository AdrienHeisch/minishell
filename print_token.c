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
		printf("ARG: |%s|\n", ft_string_get(&token->data.arg.string));
	else if (token->type == TK_PIPE)
		printf("PIPE\n");
	else if (token->type == TK_REDIR)
	{
		printf("REDIR: ");
		if (token->data.redir.type == REDIR_OUT)
			printf("> ");
		else
			exit(MS_UNREACHABLE);
		printf("%s\n", token->data.redir.file_name.content);
	}
	else
		exit(MS_UNREACHABLE);
}
