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

int			g_received_signal;

/// Will return null and errno will be set on error
static char	**dup_env(char **envp)
{
	char	**dup;
	size_t	len;
	size_t	idx;
	char	*cwd;
	char	*shlvl;
	char	*new_shlvl;

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
		if (!dup[idx])
			return (NULL);
		idx++;
	}
	dup[len] = 0;
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		if (ft_setenv(&dup, "PWD", cwd, true))
			return (NULL);
	}
	free(cwd);
	shlvl = ft_getenv(dup, "SHLVL");
	if (shlvl)
	{
		new_shlvl = ft_itoa(ft_atoi(shlvl) + 1);
		if (!new_shlvl)
			return (NULL);
		if (ft_setenv(&dup, "SHLVL", new_shlvl, true))
			return (NULL);
		free(new_shlvl);
	}
	else
	{
		if (ft_setenv(&dup, "SHLVL", "0", true))
			return (NULL);
	}
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
			exit(ERR_UNREACHABLE);
		list[idx] = ft_strdup(split[0]);
		if (!list[idx])
			return (NULL);
		free_tab((void ***)&split);
		idx++;
	}
	return (list);
}

/// Will return null and errno will be set on error
static char	*make_prompt(char **envp)
{
	t_string	prompt;
	char		*s;
	char		*h;

	prompt = ft_string_new();
	if (!prompt.content)
		return (NULL);
	if (!ft_string_cat(&prompt, "\033[36m"))
		return (NULL);
	s = ft_getenv(envp, "USER");
	if (!s)
		return (ft_string_destroy(&prompt), "$ ");
	if (!ft_string_cat(&prompt, s))
		return (NULL);
	if (!ft_string_cat(&prompt, "@"))
		return (NULL);
	s = ft_getenv(envp, "HOSTNAME");
	if (!s)
		return (ft_string_destroy(&prompt), "$ ");
	if (!ft_string_cat(&prompt, s))
		return (NULL);
	if (!ft_string_cat(&prompt, ":"))
		return (NULL);
	s = ft_getenv(envp, "PWD");
	if (!s)
		return (ft_string_destroy(&prompt), "$ ");
	h = ft_getenv(envp, "HOME");
	if (h && !ft_strncmp(s, h, ft_strlen(h)))
	{
		if (!ft_string_cat(&prompt, "~"))
			return (NULL);
		if (!ft_string_cat(&prompt, s + ft_strlen(h)))
			return (NULL);
	}
	else
	{
		if (!ft_string_cat(&prompt, s))
			return (NULL);
	}
	if (!ft_string_cat(&prompt, "\033[0m"))
		return (NULL);
	if (!ft_string_cat(&prompt, "$ "))
		return (NULL);
	return (prompt.content);
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
	{
		ft_lstclear(&tokens, (void (*)(void *))free_token);
		data->status = ERR_SYNTAX_ERROR;
		return (true);
	}
	expr = parse(&tokens);
	if (errno)
		return (print_error(), true);
	if (tokens)
	{
		ft_lstclear(&tokens, (void (*)(void *))free_token);
		data->status = ERR_SYNTAX_ERROR;
		return (true);
	}
	ft_lstclear(&tokens, (void (*)(void *))free_token);
	if (!expr)
	{
		data->status = ERR_SYNTAX_ERROR;
		return (true);
	}
	err = exec_expr(expr, data);
	if (err)
	{
		print_error();
		data->status = err;
	}
	add_history(str->content);
	return (free_expr(expr), false);
}

void	free_shell_data(t_shell_data *shell_data)
{
	free_tab((void ***)&shell_data->envp);
	free_tab((void ***)&shell_data->exported);
}

int	main(int argc, char **argv, char **envp)
{
	t_string		str;
	t_shell_data	data;
	char			**tab;
	size_t			idx;
	struct termios	tio;
	char			*prompt;

	g_received_signal = 0;
	if (init_signals())
		return (print_error(), ERR_SYSTEM);
	data.envp = dup_env(envp);
	if (!data.envp)
		return (print_error(), ERR_SYSTEM);
	data.exported = make_export_list(data.envp);
	if (!data.exported)
		return (print_error(), ERR_SYSTEM);
	if (export_var(&data.exported, "OLDPWD"))
		return (print_error(), ERR_SYSTEM);
	if (argc >= 1)
		ft_setenv(&data.envp, "_", argv[0], true);
	data.status = 0;
	if (argc == 3 && ft_strncmp("-c", argv[1], 3) == 0)
	{
		tab = ft_split(argv[2], '\n');
		if (!tab)
			return (ERR_SYSTEM);
		idx = 0;
		while (tab[idx])
		{
			str = ft_string_from(tab[idx]);
			if (parse_and_exec(&str, &data))
				break ;
			ft_string_destroy(&str);
			idx++;
		}
		free(tab);
		free_shell_data(&data);
		return (data.status);
	}
	if (argc > 1)
		return (free_shell_data(&data), ERR_USAGE);
	if (isatty(STDIN_FILENO))
		tio = set_terminal_attributes();
	rl_outstream = stderr;
	str = (t_string){0};
	while (1)
	{
		ft_string_destroy(&str);
		prompt = make_prompt(data.envp);
		if (!prompt)
			print_error();
		if (isatty(STDIN_FILENO))
		{
			if (USE_READLINE)
				str = ft_string_from(readline(prompt));
			else
				str = readline_lite(prompt);
		}
		else
			str = readline_lite(NULL);
		if (g_received_signal > 0)
		{
			data.status = 128 + g_received_signal;
			g_received_signal = 0;
		}
		if (!str.content)
			break ;
		if (parse_and_exec(&str, &data) && !isatty(STDIN_FILENO))
			break ;
		g_received_signal = 0;
	}
	ft_string_destroy(&str);
	if (isatty(STDIN_FILENO))
		restore_terminal_attributes(&tio);
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", STDERR_FILENO);
	free_shell_data(&data);
	return (data.status);
}
