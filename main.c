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
#include <errno.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static void	on_syntax_error(t_list **tokens, t_shell_data *data)
{
	ft_lstclear(tokens, (void (*)(void *))free_token);
	print_error_msg("syntax error");
	data->status = ERR_SYNTAX_ERROR;
}

bool	parse_and_exec(t_string str, t_shell_data *data)
{
	t_list	*tokens;
	t_expr	*expr;
	t_err	err;

	if (is_only_whitespace(&str))
		return (ft_string_destroy(&str), data->status = ERR_OK, false);
	(add_history(str.content), err = lex(&str, &tokens));
	ft_string_destroy(&str);
	if (err == ERR_SYSTEM)
		return (print_error(), true);
	if (err == ERR_SYNTAX_ERROR)
		return (on_syntax_error(&tokens, data), true);
	expr = parse(&tokens);
	if (errno)
		return (print_error(), true);
	if (tokens)
		return (on_syntax_error(&tokens, data), true);
	ft_lstclear(&tokens, (void (*)(void *))free_token);
	if (!expr)
		return (on_syntax_error(&tokens, data), true);
	data->ast_root = expr;
	err = exec_expr(expr, data);
	if (err)
		(print_error(), data->status = err);
	return (free_expr(expr), data->ast_root = NULL, false);
}

static t_string	read_input(t_shell_data *data)
{
	char		*prompt;
	t_string	str;

	prompt = make_prompt(data->envp);
	if (!prompt)
		print_error();
	errno = 0;
	if (isatty(STDIN_FILENO))
	{
		if (USE_READLINE)
			str = ft_string_from(readline(prompt));
		else
			str = readline_lite(prompt);
	}
	else
		str = readline_lite(NULL);
	free(prompt);
	return (str);
}

static void	run(t_shell_data *data)
{
	t_string		str;
	struct termios	tio;

	if (isatty(STDIN_FILENO))
		tio = set_terminal_attributes();
	rl_outstream = stderr;
	str = (t_string){0};
	while (true)
	{
		str = read_input(data);
		if (errno)
			print_error();
		if (!str.content || (parse_and_exec(str, data) && !isatty(0)))
			break ;
	}
	if (isatty(STDIN_FILENO))
		(restore_terminal_attributes(&tio), ft_putstr_fd("exit\n", 2));
}

int	main(int argc, char **argv, char **envp)
{
	t_shell_data	data;

	if (init_signals())
		return (print_error(), ERR_SYSTEM);
	data.envp = dup_env(envp);
	if (!data.envp)
		return (print_error(), ERR_SYSTEM);
	data.exported = make_export_list(data.envp);
	if (!data.exported || export_var(&data.exported, "OLDPWD"))
		return (print_error(), ERR_SYSTEM);
	if (argc >= 1)
		ft_setenv(&data.envp, "_", argv[0], true);
	data.status = 0;
	data.ast_root = NULL;
	if (argc == 3 && ft_strncmp("-c", argv[1], 3) == 0)
	{
		if (argument_mode(argv, &data))
			return (free_shell_data(&data), ERR_SYSTEM);
	}
	else if (argc > 1)
		return (free_shell_data(&data), ERR_USAGE);
	else
		run(&data);
	return (free_shell_data(&data), data.status);
}
