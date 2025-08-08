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

void	print_token(t_token *token)
{
	if (token->type == TK_ARG)
	{
		if (token->data.arg.expand)
			printf("$ ");
		if (token->data.arg.is_dq)
			printf("DQ ");
		printf("ARG: |%s|\n", ft_string_get(&token->data.arg.string));
	}
	else if (token->type == TK_PIPE)
		printf("PIPE\n");
}
