/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 23:57:23 by aheisch           #+#    #+#             */
/*   Updated: 2025/10/08 23:57:23 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static char	*init_process_heredoc_delim(char *delim, char **quote,
		t_string *out)
{
	errno = 0;
	*quote = ft_strchr(delim, '"');
	if (!*quote)
		*quote = ft_strchr(delim, '\'');
	if (!*quote)
		return (NULL);
	*out = ft_string_new();
	if (!out->content)
		return (NULL);
	return (delim);
}

/// errno will only be set on error
static char	*process_heredoc_delim(char *delim)
{
	char		*quote;
	char		*s;
	t_string	out;
	char		*rec;

	quote = NULL;
	s = init_process_heredoc_delim(delim, &quote, &out);
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s != *quote)
		{
			if (!ft_string_ncat(&out, s, 1))
				return (NULL);
		}
		s++;
	}
	rec = process_heredoc_delim(out.content);
	if (errno)
		return (NULL);
	if (rec)
		return (rec);
	return (out.content);
}

t_err	expand_it_then(t_string *line, int fd_out, t_shell_data *shell_data)
{
	t_list	*expanded;
	t_list	*o_expanded;

	o_expanded = expand_arg(line, shell_data, true);
	if (errno)
	{
		return (ERR_SYSTEM);
	}
	expanded = o_expanded;
	while (expanded && expanded->content)
	{
		ft_putstr_fd(((t_string *)expanded->content)->content, fd_out);
		expanded = expanded->next;
	}
	ft_lstclear(&o_expanded, lstclear_string);
	return (ERR_OK);
}

extern int	g_heredoc_exit_flag;

t_err	while_heredoc(char *no_expand, int fd_out, char *delim,
		t_shell_data *shell_data)
{
	t_string	line;

	g_heredoc_exit_flag = false;
	errno = 0;
	if (isatty(STDIN_FILENO) && USE_READLINE)
		line = ft_string_from(readline("> "));
	if (isatty(STDIN_FILENO) && !USE_READLINE)
		line = readline_lite("> ");
	if (!isatty(STDIN_FILENO))
		line = readline_lite(NULL);
	if (errno)
		return (ERR_SYSTEM);
	if (!line.content || g_heredoc_exit_flag)
		return (111999);
	if (!ft_strncmp(line.content, delim, size_t_max(line.length,
				ft_strlen(delim))))
		return (ft_string_destroy(&line), 111999);
	if (no_expand)
		ft_putstr_fd(line.content, fd_out);
	else if (expand_it_then(&line, fd_out, shell_data))
		return (ft_string_destroy(&line), ERR_SYSTEM);
	ft_putstr_fd("\n", fd_out);
	return (ft_string_destroy(&line), ERR_OK);
}

/// Returns ERR_OK or ERR_SYSTEM
t_err	prompt_heredoc(int fd_in, int fd_out, char *delim,
		t_shell_data *shell_data)
{
	char	*no_expand;
	t_err	err;
	int		pid;

	no_expand = process_heredoc_delim(delim);
	if (errno)
		return (ERR_SYSTEM);
	if (no_expand)
		delim = no_expand;
	pid = fork();
	if (pid != 0)
		return (waitpid(pid, NULL, 0), ERR_OK);
	init_signals_heredoc();
	while (1)
	{
		err = while_heredoc(no_expand, fd_out, delim, shell_data);
		if (err == 111999)
			break ;
		if (err)
			(close_redirections(fd_in, fd_out), free_shell_data(shell_data),
				exit(err));
	}
	if (no_expand)
		free(delim);
	(close_redirections(fd_in, fd_out), free_shell_data(shell_data), exit(0));
}
