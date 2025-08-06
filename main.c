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
		while (*idx + len < str->length && str->content[*idx + len] >= 'a'
			&& str->content[*idx + len] <= 'z')
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
	if (token->type == TK_ARG)
		ft_string_destroy(&token->data.arg.string);
	free(token);
}

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
	}
	else if (expr->type == EX_PIPE)
	{
		printf("PIPE_START\n");
		print_expr(expr->data.pipe.left);
		print_expr(expr->data.pipe.right);
		printf("PIPE_END");
	}
	printf("\n");
}

t_expr	*parse_cmd(t_list **tokens)
{
	t_expr		*expr;
	t_list		*token;
	t_string	*string;
	t_list		*arg;

	expr = malloc(sizeof(t_expr));
	if (!expr)
		exit(MS_ALLOC);
	expr->type = EX_CMD;
	while (*tokens && ((t_token *)(*tokens)->content)->type == TK_ARG)
	{
		token = ft_lstpop_front(tokens);
		string = malloc(sizeof(t_string));
		if (!string)
			exit(MS_ALLOC);
		ft_string_move(&((t_token *)token->content)->data.arg.string, string);
		arg = ft_lstnew(string);
		ft_lstadd_back(&expr->data.cmd.args, arg);
		ft_lstdelone(token, (void (*)(void *))free_token);
	}
	return (expr);
}

t_expr	*parse_pipe(t_list **tokens, t_list **exprs)
{
	t_expr		*expr;
	t_list		*token;

	expr = malloc(sizeof(t_expr));
	if (!expr)
		exit(MS_ALLOC);
	expr->type = EX_PIPE;
	token = ft_lstpop_front(tokens);
	ft_lstdelone(token, (void (*)(void *))free_token);
	expr->data.pipe.left = ft_lstpop_back(exprs)->content;
	expr->data.pipe.right = parse_cmd(tokens);
	return (expr);
}

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

int	main(int argc, char **argv)
{
	t_string	str;
	t_list		*exprs;

	(void)argc;
	str = ft_string_new();
	ft_string_cat(&str, argv[1]);
	exprs = parse(&str);
	ft_lstiter(exprs, (void (*)(void *))print_expr);
	ft_string_destroy(&str);
	return (0);
}
