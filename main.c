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

int				g_received_signal;

/// Returns ERR_OK or ERR_SYSTEM
static t_err	increment_shlvl(char ***dup)
{
	char	*shlvl;
	char	*new_shlvl;

	shlvl = ft_getenv(*dup, "SHLVL");
	if (shlvl)
	{
		new_shlvl = ft_itoa(ft_atoi(shlvl) + 1);
		if (!new_shlvl || ft_setenv(dup, "SHLVL", new_shlvl, true))
			return (free(new_shlvl), ERR_SYSTEM);
		free(new_shlvl);
	}
	else if (ft_setenv(dup, "SHLVL", "0", true))
		return (ERR_SYSTEM);
	return (ERR_OK);
}

/// Will return null and errno will be set on error
static char	**dup_env(char **envp)
{
	char	**dup;
	size_t	len;
	size_t	idx;
	char	*cwd;

	len = 0;
	while (envp[len])
		len++;
	dup = ft_calloc(len + 1, sizeof(char *));
	if (!dup)
		return (NULL);
	idx = 0;
	while (envp[idx])
	{
		dup[idx] = ft_strdup(envp[idx]);
		if (!dup[idx++])
			return (free_tab((void ***)&dup), NULL);
	}
	cwd = getcwd(NULL, 0);
	if (cwd && ft_setenv(&dup, "PWD", cwd, true))
		return (free_tab((void ***)&dup), free(cwd), NULL);
	free(cwd);
	if (increment_shlvl(&dup))
		return (free_tab((void ***)&dup), NULL);
	return (dup);
}

/// Will return null and errno will be set on error
static char	**make_export_list(char **envp)
{
	char	**list;
	size_t	len;
	size_t	idx;
	char	**split;

	len = 0;
	while (envp[len])
		len++;
	list = ft_calloc(len + 1, sizeof(char *));
	if (!list)
		return (NULL);
	idx = 0;
	while (envp[idx])
	{
		split = ft_split(envp[idx], '=');
		if (!split)
			return (NULL);
		if (!split[0])
			continue ;
		list[idx] = ft_strdup(split[0]);
		if (!list[idx++])
			return (NULL);
		free_tab((void ***)&split);
	}
	return (list);
}

/// Returns true if execution should be interrupted in non interactive mode
static bool	parse_and_exec(t_string *str, t_shell_data *data)
{
	t_list	*tokens;
	t_expr	*expr;
	t_err	err;

	if (is_only_whitespace(str))
		return (data->status = ERR_OK, false);
	err = lex(str, &tokens);
	if (err == ERR_SYSTEM)
		return (print_error(), true);
	if (err == ERR_SYNTAX_ERROR)
		return (ft_lstclear(&tokens, (void (*)(void *))free_token),
			data->status = ERR_SYNTAX_ERROR, true);
	expr = parse(&tokens);
	if (errno)
		return (print_error(), true);
	if (tokens)
		return (ft_lstclear(&tokens, (void (*)(void *))free_token),
			data->status = ERR_SYNTAX_ERROR, true);
	ft_lstclear(&tokens, (void (*)(void *))free_token);
	if (!expr)
		return (data->status = ERR_SYNTAX_ERROR, true);
	err = exec_expr(expr, data);
	if (err)
		(print_error(), data->status = err);
	return (add_history(str->content), free_expr(expr), false);
}

void	free_shell_data(t_shell_data *shell_data)
{
	free_tab((void ***)&shell_data->envp);
	free_tab((void ***)&shell_data->exported);
}

static t_err	argument_mode(char **argv, t_shell_data *data)
{
	t_string	str;
	char		**tab;
	size_t		idx;

	tab = ft_split(argv[2], '\n');
	if (!tab)
		return (ERR_SYSTEM);
	idx = 0;
	while (tab[idx])
	{
		str = ft_string_from(tab[idx++]);
		if (parse_and_exec(&str, data))
			break ;
		ft_string_destroy(&str);
	}
	return (free(tab), ft_string_destroy(&str), ERR_OK);
}

static t_string	read_input(t_shell_data *data)
{
	char		*prompt;
	t_string	str;

	prompt = make_prompt(data->envp);
	if (!prompt)
		print_error();
	errno = 0;
	if (isatty(STDIN_FILENO) && USE_READLINE)
		str = ft_string_from(readline(prompt));
	if (isatty(STDIN_FILENO) && !USE_READLINE)
		str = readline_lite(prompt);
	if (!isatty(STDIN_FILENO))
		str = readline_lite(NULL);
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
		ft_string_destroy(&str);
		str = read_input(data);
		if (errno)
			print_error();
		if (g_received_signal > 0)
		{
			data->status = 128 + g_received_signal;
			g_received_signal = 0;
		}
		if (!str.content || (parse_and_exec(&str, data) && !isatty(0)))
			break ;
		g_received_signal = 0;
	}
	if (isatty(STDIN_FILENO))
		(restore_terminal_attributes(&tio), ft_putstr_fd("exit\n", 2));
	ft_string_destroy(&str);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell_data	data;

	g_received_signal = 0;
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
