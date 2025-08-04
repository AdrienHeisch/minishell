/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:47:53 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/04 15:47:53 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

t_token	*get_token(t_string *str, size_t *idx)
{
	t_token	token;
	t_token	*cell;
	size_t	len;
	char	c;

	len = 0;
	c = str->content[*idx];
	if (c >= 'a' && c <= 'z')
	{
		while (*idx + len < str->length && str->content[*idx + len] >= 'a' && str->content[*idx
			+ len] <= 'z')
		{
			len++;
		}
		token.type = TK_ARG;
		token.data.arg.string = ft_string_new();
		ft_string_ncat(&token.data.arg.string, &str->content[*idx], len);
	}
	else if (c == '|')
	{
		token.type = TK_PIPE;
		len = 1;
	}
	else
	{
		return (NULL);
	}
	*idx += len;
	cell = malloc(sizeof(t_token));
	*cell = token;
	return (cell);
}

t_list	*lex(t_string *str)
{
	t_list	*tokens;
	t_list	*new_list;
	t_token	*token;
	size_t	idx;

	tokens = NULL;
	idx = 0;
	while (idx < str->length)
	{
		if (str->content[idx] == ' ')
		{
			idx++;
			continue ;
		}
		token = get_token(str, &idx);
		if (!token)
			break ;
		new_list = ft_lstnew(token);
		if (!new_list)
			break ; // TODO ????
		ft_lstadd_back(&tokens, new_list);
	}
	printf("oui %d\n", ft_lstsize(tokens));
	return (tokens);
}

void	print_token(t_token *token)
{
	if (token->type == TK_ARG)
		printf("ARG: %s\n", ft_string_get(&token->data.arg.string));
	else if (token->type == TK_PIPE)
		printf("PIPE\n");
}

void	free_token(t_token *token)
{
	(void)token;
	// TODO
}

t_expr	parse(t_string *str)
{
	t_list	*tokens;
	t_list	*token;
	t_list	*exprs;
	t_expr	expr;

	tokens = lex(str);
	while (tokens)
	{
		token = ft_lstpop_front(&tokens);
		if (((t_token *)token->content)->type == TK_ARG)
		{
			expr.type = EX_CMD;
			ft_lstdelone(token, (void (*)(void *))free_token);
			while (((t_token *)tokens->content)->type == TK_ARG)
			{

			}
		}

	}
	exit(0);
}

int	main(int argc, char **argv)
{
	t_string	str;

	(void)argc;
	str = ft_string_new();
	ft_string_cat(&str, argv[1]);
	parse(&str);
	ft_string_destroy(&str);
	return (0);
}
