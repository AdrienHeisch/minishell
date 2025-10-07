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

# ifndef USE_READLINE
#  define USE_READLINE false
# endif // !USE_READLINE

# include "libft.h"
# include <stdbool.h>
# include <stdio.h>

// If a function returns ERR_SYSTEM, errno will always be set
# define ERR_SYSTEM 255

# define ERR_OK 0
# define ERR_COMMAND_FAILED 1
# define ERR_SYNTAX_ERROR 2
# define ERR_USAGE 3
# define ERR_UNREACHABLE 4 // TODO remove this
# define ERR_LOGIC 5       // TODO remove this

# define ERR_PERMISSION 126
# define ERR_COMMAND_NOT_FOUND 127

typedef int					t_err;

extern int					g_received_signal;

typedef struct s_shell_data
{
	char					**envp;
	char					**exported;
	t_err					status;
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
	} u_data;
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
	} u_data;
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
	t_err					error;
}							t_exec_info;

enum						e_control_flow
{
	CF_NONE,
	CF_CONTINUE,
	CF_RETURN_ERR,
};

struct						s_expand
{
	t_list					*out;
	t_string				exp;
	char					del;
	size_t					idx;
	bool					has_empty_var;
};

void						free_shell_data(t_shell_data *shell_data);

char						*make_prompt(char **envp);

t_err						init_signals(void);

struct termios				set_terminal_attributes(void);
void						restore_terminal_attributes(struct termios *og_tio);

t_list						*expand_arg(t_string *arg, t_shell_data *shell_data,
								bool is_heredoc);

void						print_token(t_token *token);
void						free_token(t_token *token);
void						print_expr(t_expr *expr);
void						free_expr(t_expr *expr);
t_err						lex(t_string *str, t_list **tokens);
t_expr						*parse(t_list **tokens);
t_expr						*parse_expr(t_list **tokens, t_expr **prev);
t_expr						*parse_cmd(t_list **tokens);
t_expr						*parse_binop(t_list **tokens, t_expr **prev);
t_expr						*parse_parentheses(t_list **tokens);
t_err						exec_binop(t_binop binop, t_shell_data *shell_data);
t_err						exec_expr(t_expr *expr, t_shell_data *shell_data);
t_err						exec_cmd(t_expr *expr, t_shell_data *shell_data);
t_err						exec_parentheses(t_expr *expr,
								t_shell_data *shell_data);
t_err						exec_pipe(t_binop pipe, t_shell_data *shell_data);
void						run_cmd(t_exec_info cmd, t_shell_data *shell_data);
int							fork_run_cmd(t_exec_info exec,
								t_shell_data *shell_data);

bool						is_builtin(char *name);
t_err						exec_builtin(t_exec_info args,
								t_shell_data *shell_data);
t_err						builtin_cd(char **args, t_shell_data *shell_data);
t_err						builtin_echo(char **args, t_shell_data *shell_data,
								int fd_out);
t_err						builtin_env(char **args, t_shell_data *shell_data,
								int fd_out);
t_err						builtin_exit(char **args, t_shell_data *shell_data);
t_err						builtin_export(char **args,
								t_shell_data *shell_data, int fd_out);
int							allowed_char(int c);
t_err						is_valid_var(char *name);

char						*make_prompt(char **envp);

t_err						builtin_unset(char **args,
								t_shell_data *shell_data);
t_err						builtin_pwd(char **args, t_shell_data *shell_data,
								int fd_out);
t_err						export_var(char ***exported, const char *name);

char						**make_arg_list(t_cmd cmd,
								t_shell_data *shell_data);

t_exec_info					make_exec_info(t_cmd cmd, int fd_in, int fd_out,
								t_shell_data *shell_data);
void						free_exec_info(t_exec_info *exec);
int							resolve_exec_path(char **cmd,
								t_shell_data *shell_data);

t_err						add_redirection(t_list *token, t_list **list);
t_err						resolve_redirections(t_expr *expr,
								t_shell_data *shell_data);
void						close_redirections(int fd_in, int fd_out);

void						print_error_msg(char *err);
void						print_error(void);
void						print_error_prefix(char *prefix);
void						print_error_code(char *path, int code);

void						no_op(void *p);
void						lstclear_string(void *str);
bool						is_whitespace(char c);
bool						is_only_whitespace(t_string *str);
bool						is_str_all(char *s, int f(int));
char						*ft_getenv(char **envp, const char *name);
t_err						ft_setenv(char ***envp, const char *name,
								const char *value, int overwrite);
t_err						ft_unsetenv(char ***envp, const char *name);
t_string					readline_lite(char *prompt);
t_err						prompt_heredoc(int out, char *delim,
								t_shell_data *shell_data);
t_err						find_options(int *flags, char **args, size_t *idx,
								char *options);
t_err						lstadd_back_string(t_list **list, t_string str);
void						free_tab(void ***tab);

t_list						*expand_wildcards(char *pattern);

#endif // !MINISHELL_H
