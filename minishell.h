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

extern int					received_signal;

enum						e_error
{
	MS_SUCCESS,
	MS_USAGE,
	MS_ALLOC,
	MS_UNREACHABLE,
};

typedef struct s_shell_data
{
	char					**envp;
	int						status;
}							t_shell_data;

typedef enum s_token_type
{
	TK_INVALID,
	TK_ARG,
	TK_PIPE,
	TK_REDIR,
}							t_token_type;

typedef enum s_redir_type
{
	REDIR_IN,
	REDIR_HEREDOC,
	REDIR_OUT,
	REDIR_APPEND,
}							t_redir_type;

typedef struct s_token
{
	t_token_type			type;
	union
	{
		struct				s_arg_data
		{
			t_string		string;
		} arg;
		struct				s_redir_data
		{
			t_redir_type	type;
			int				fd;
			t_string		file_name;
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
			int				fd_in;
			int				fd_out;
			t_list			*redirs;
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

void						init_signals(void);

struct termios				set_terminal_attributes(void);
void						restore_terminal_attributes(struct termios *original_tio);

void						print_token(t_token *token);
void						free_token(t_token *token);
void						print_expr(t_expr *expr);
void						free_expr(t_expr *expr);
t_list						*lex(t_string *str);
t_list						*parse(t_string *str);
t_expr						*parse_expr(t_list **tokens, t_list **exprs);
t_expr						*parse_cmd(t_list **tokens);
t_expr						*parse_pipe(t_list **tokens, t_list **exprs);
t_expr						*parse_redir_in(t_list **tokens, t_list **exprs);
void						exec(t_expr *expr, t_shell_data *shell_data);
void						exec_cmd(t_cmd cmd, t_shell_data *shell_data);
int							exec_pipe(t_pipe pipe, t_shell_data *shell_data);
int							fork_exec_cmd(t_cmd cmd, t_shell_data *shell_data);

bool						is_builtin(t_string *name);
bool						exec_builtin(t_cmd cmd, t_shell_data *shell_data);
void						builtin_cd(char **args, t_shell_data *shell_data);
void						builtin_echo(char **args, t_shell_data *shell_data,
								int fd_out);
void						builtin_env(t_shell_data *shell_data, int fd_out);
void						builtin_exit(char **args, t_shell_data *shell_data);
void						builtin_export(char **args,
								t_shell_data *shell_data, int fd_out);
void						builtin_unset(char **args,
								t_shell_data *shell_data);
void						builtin_pwd(char **args, t_shell_data *shell_data,
								int fd_out);

char						**make_arg_list(t_cmd cmd,
								t_shell_data *shell_data);
void						free_args_list(char **args);

int							resolve_redirections(t_cmd *cmd);
void						close_redirections(t_cmd *cmd);

void						no_op(void *p);
void						lstclear_string(void *str);
bool						is_whitespace(t_string *str);
char						*ft_getenv(char **envp, const char *name);
void						ft_setenv(char ***envp, const char *name,
								const char *value, int overwrite);
void						ft_unsetenv(char ***envp, const char *name);

#endif // !MINISHELL_H
