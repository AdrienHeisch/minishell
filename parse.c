/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:01:13 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/06 13:01:13 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

t_list	*parse(t_string *str)
{
	t_list	*tokens;
	t_list	*exprs;

	tokens = lex(str);
	exprs = NULL;
	while (tokens)
	{
		if (((t_token *)tokens->content)->type == TK_ARG)
			ft_lstadd_back(&exprs, ft_lstnew(parse_cmd(&tokens)));
		else if (((t_token *)tokens->content)->type == TK_PIPE)
			ft_lstadd_back(&exprs, ft_lstnew(parse_pipe(&tokens, &exprs)));
	}
	return (exprs);
}
