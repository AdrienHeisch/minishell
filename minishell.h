/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:00:59 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/27 14:58:03 by galauren         ###   ########.fr       */
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
	MS_COMMAND_FAILED,
	MS_SYNTAX_ERROR,
	MS_USAGE,
	MS_ALLOC,
	MS_UNREACHABLE,
	MS_LOGIC_ERROR,
};

typedef struct s_shell_data
{
	char					**envp;
	char					**exported;
	int						status;
}							t_shell_data;

typedef enum s_token_type
{
	TK_INVALID,
	TK_ARG,
	TK_PIPE,
	TK_REDIR,
	TK_AND,
	TK_OR,
	TK_PAROPEN,
	TK_PARCLOSE,
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
	EX_BINOP,
	EX_PARENTHESES,
}							t_expr_type;

typedef enum e_operator
{
	OP_PIPE,
	OP_AND,
	OP_OR,
}							t_operator;

typedef struct s_expr
{
	t_expr_type				type;
	union
	{
		struct				s_cmd
		{
			t_list			*args;
		} cmd;
		struct				s_binop
		{
			struct s_expr	*left;
			struct s_expr	*right;
			t_operator		op;
		} binop;
		struct				s_paren
		{
			struct s_expr	*inner;
		} paren;
	} data;
	int						fd_in;
	int						fd_out;
	t_list					*redirs;
}							t_expr;

typedef struct s_cmd		t_cmd;
typedef struct s_binop		t_binop;
typedef struct s_paren		t_paren;

typedef struct s_exec_info
{
	char					**args;
	int						fd_in;
	int						fd_out;
	int						error;
}							t_exec_info;

void						free_shell_data(t_shell_data *shell_data);

void						init_signals(void);

struct termios				set_terminal_attributes(void);
void						restore_terminal_attributes(struct termios *original_tio);

void						print_token(t_token *token);
void						free_token(t_token *token);
void						print_expr(t_expr *expr);
void						free_expr(t_expr *expr);
t_list						*lex(t_string *str);
t_expr						*parse(t_list **tokens);
t_expr						*parse_expr(t_list **tokens, t_expr **prev);
t_expr						*parse_cmd(t_list **tokens);
t_expr						*parse_binop(t_list **tokens, t_expr **prev);
t_expr						*parse_parentheses(t_list **tokens);
void						exec_binop(t_binop binop, t_shell_data *shell_data);
void						exec_expr(t_expr *expr, t_shell_data *shell_data);
void						exec_cmd(t_expr *expr, t_shell_data *shell_data);
void						exec_parentheses(t_expr *expr,
								t_shell_data *shell_data);
void						exec_pipe(t_binop pipe, t_shell_data *shell_data);
void						run_cmd(t_exec_info cmd, t_shell_data *shell_data);
int							fork_run_cmd(t_exec_info exec,
								t_shell_data *shell_data);

bool						is_builtin(char *name);
bool						exec_builtin(t_exec_info args,
								t_shell_data *shell_data);
void						builtin_cd(char **args, t_shell_data *shell_data);
void						builtin_echo(char **args, t_shell_data *shell_data,
								int fd_out);
void						builtin_env(char **args, t_shell_data *shell_data,
								int fd_out);
void						builtin_exit(char **args, t_shell_data *shell_data);
void						builtin_export(char **args,
								t_shell_data *shell_data, int fd_out);
void						builtin_unset(char **args,
								t_shell_data *shell_data);
void						builtin_pwd(char **args, t_shell_data *shell_data,
								int fd_out);
void						export_var(char ***exported, const char *name);

t_list						*expand_arg(t_string *arg,
								t_shell_data *shell_data);
char						**make_arg_list(t_cmd cmd,
								t_shell_data *shell_data);

t_exec_info					make_exec_info(t_cmd cmd, int fd_in, int fd_out,
								t_shell_data *shell_data);
void						free_exec_info(t_exec_info *exec);
int							resolve_exec_path(char **cmd,
								t_shell_data *shell_data);

void						add_redirection(t_list *token, t_list **list);
int							resolve_redirections(t_expr *expr,
								t_shell_data *shell_data);
void						close_redirections(int fd_in, int fd_out);

void						print_error(char *err);
void						print_error_code(char *path, int code);

void						no_op(void *p);
void						lstclear_string(void *str);
bool						is_whitespace(char c);
bool						is_only_whitespace(t_string *str);
bool						is_str_all(char *s, int f(int));
char						*ft_getenv(char **envp, const char *name);
void						ft_setenv(char ***envp, const char *name,
								const char *value, int overwrite);
void						ft_unsetenv(char ***envp, const char *name);
t_string					readline_lite(void);
void						prompt_heredoc(int out, char *delim,
								t_shell_data *shell_data);
int							find_options(int *flags, char **args, size_t *idx,
								char *options);
void						lstadd_back_string(t_list **list, t_string str);
void						free_tab(void **tab);

t_list						*expand_wildcards(char *pattern);

#endif // !MINISHELL_H
