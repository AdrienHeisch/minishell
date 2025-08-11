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
# include <stdbool.h>
# include <stdio.h>

enum						e_error
{
	MS_SUCCESS,
	MS_USAGE,
	MS_ALLOC,
};

typedef struct s_shell_data
{
	char					**envp;
	int						status;
}							t_shell_data;

typedef enum s_token_type
{
	TK_ARG,
	TK_PIPE,
	TK_REDIR_IN,
	TK_REDIR_OUT,
}							t_token_type;

typedef struct s_token
{
	t_token_type			type;
	union
	{
		struct				s_arg_data
		{
			t_string		string;
			bool			expand;
			bool			is_dq;
		} arg;
		struct				s_redir_data
		{
			int				fd;
		} redir;
	} data;
}							t_token;

typedef struct s_arg_data	t_arg_data;
typedef struct s_redir_data	t_redir_data;

typedef enum s_expr_type
{
	EX_CMD,
	EX_PIPE,
}							t_expr_type;

typedef struct s_expr
{
	t_expr_type				type;
	union
	{
		struct				s_cmd
		{
			t_list			*args;
			// t_list			*redirs_in; // TODO list of redirections
			int				fd_in;
			int				fd_out;
			t_string		file_in;
			t_string		file_out;
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
void						free_expr(t_expr *expr);
t_list						*lex(t_string *str);
t_expr						*parse_cmd(t_list **tokens);
t_expr						*parse_pipe(t_list **tokens, t_list **exprs);
t_expr						*parse_redir_in(t_list **tokens, t_list **exprs);
t_list						*parse(t_string *str);
void						exec(t_expr *expr, t_shell_data *shell_data);
void						exec_cmd(t_cmd cmd, t_shell_data *shell_data);
int							exec_pipe(t_pipe pipe, t_shell_data *shell_data);
void						child_last(t_cmd cmd, t_shell_data *shell_data,
								int prev_fd, int outfile);
bool						exec_builtin(char **args, t_shell_data *shell_data);

void						builtin_echo(char **args, t_shell_data *shell_data);

void						lstclear_string(void *str);
bool						is_whitespace(t_string *str);

#endif // !MINISHELL_H
