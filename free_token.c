/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 12:58:49 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/06 12:58:49 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

void	free_token(t_token *token)
{
	if (token->type == TK_ARG)
		ft_string_destroy(&token->data.arg.string);
	if (token->type == TK_REDIR)
		ft_string_destroy(&token->data.redir.file_name);
	free(token);
}
