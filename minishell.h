/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:00:59 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/04 16:00:59 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

enum						e_error
{
	MS_SUCCESS,
	MS_USAGE,
	MS_ALLOC,
};

typedef enum s_token_type
{
	TK_ARG,
	TK_PIPE,
}							t_token_type;

typedef struct s_token
{
	t_token_type			type;
	union
	{
		struct
		{
			t_string		string;
		} arg;
	} data;
}							t_token;

typedef enum s_expr_type
{
	EX_CMD,
	EX_PIPE
}							t_expr_type;

typedef struct s_expr
{
	t_expr_type				type;
	union
	{
		struct				s_cmd
		{
			t_list			*args;
		} cmd;
		struct				s_pipe
		{
			struct s_expr	*left;
			struct s_expr	*right;
		} pipe;
	} data;
}							t_expr;

typedef struct s_cmd		t_cmd;
typedef struct s_pipe		t_pipe;

void						print_token(t_token *token);
void						free_token(t_token *token);
void						print_expr(t_expr *expr);
t_list						*lex(t_string *str);
t_expr						*parse_cmd(t_list **tokens);
t_expr						*parse_pipe(t_list **tokens, t_list **exprs);
t_list						*parse(t_string *str);
void						exec_cmd(t_cmd cmd, char **envp);

#endif // !MINISHELL_H
