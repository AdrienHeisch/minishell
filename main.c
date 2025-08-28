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
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int			received_signal;

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
	idx = 0;
	while (envp[idx])
	{
		dup[idx] = ft_strdup(envp[idx]);
		idx++;
	}
	dup[len] = 0;
	cwd = getcwd(NULL, 0);
	if (cwd)
		ft_setenv(&dup, "PWD", cwd, true);
	return (dup);
}

static char	*make_prompt(char **envp)
{
	t_string	prompt;
	char		*s;

	prompt = ft_string_new();
	s = ft_getenv(envp, "USER");
	if (!s)
		return (ft_string_destroy(&prompt), "$ ");
	ft_string_cat(&prompt, s);
	ft_string_cat(&prompt, "@");
	s = ft_getenv(envp, "HOSTNAME");
	if (!s)
		return (ft_string_destroy(&prompt), "$ ");
	ft_string_cat(&prompt, s);
	ft_string_cat(&prompt, ":");
	s = ft_getenv(envp, "PWD");
	if (!s)
		return (ft_string_destroy(&prompt), "$ ");
	ft_string_cat(&prompt, s);
	ft_string_cat(&prompt, "$ ");
	return (prompt.content);
}

static void	parse_and_exec(t_string *str, t_shell_data *data)
{
	t_list	*tokens;
	t_expr	*expr;

	if (is_whitespace(str))
	{
		data->status = 0;
		return ;
	}
	tokens = lex(str);
	// ft_lstiter(tokens, (void (*)(void *))print_token);
	expr = parse(&tokens);
	if (!expr)
	{
		data->status = 2;
		return ;
	}
	exec_expr(expr, data);
	free_expr(expr);
	add_history(str->content);
}

int	main(int argc, char **argv, char **envp)
{
	t_string		str;
	t_shell_data	data;
	char			**tab;
	size_t			idx;
	struct termios	tio;
	char			*prompt;

	received_signal = 0;
	init_signals();
	data.envp = dup_env(envp);
	data.status = 0;
	if (argc == 3 && ft_strncmp("-c", argv[1], 3) == 0)
	{
		tab = ft_split(argv[2], '\n');
		idx = 0;
		while (tab[idx])
		{
			str = ft_string_new();
			ft_string_cat(&str, tab[idx]);
			parse_and_exec(&str, &data);
			ft_string_destroy(&str);
			idx++;
		}
		return (data.status);
	}
	if (argc > 1)
		return (MS_USAGE);
	tio = set_terminal_attributes();
	str.content = NULL;
	while (1)
	{
		ft_string_destroy(&str);
		prompt = make_prompt(data.envp);
		str = ft_string_from(readline(prompt));
		if (received_signal > 0)
		{
			data.status = 128 + received_signal;
			received_signal = 0;
		}
		if (!str.content)
			break ;
		parse_and_exec(&str, &data);
		received_signal = 0;
	}
	ft_string_destroy(&str);
	restore_terminal_attributes(&tio);
	return (data.status);
}
