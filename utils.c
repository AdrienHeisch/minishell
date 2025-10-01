/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:58:29 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/06 17:58:29 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <errno.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

void	no_op(void *p)
{
	(void)p;
}

void	lstclear_string(void *str)
{
	ft_string_delete((t_string **)&str);
}

bool	is_str_all(char *s, int f(int))
{
	while (*s)
	{
		if (!f(*s))
			return (false);
		s++;
	}
	return (true);
}

bool	is_whitespace(char c)
{
	static const char	*list = " \t\n";

	if (!c)
		return (false);
	return (ft_strchr(list, c) != NULL);
}

bool	is_only_whitespace(t_string *str)
{
	size_t	idx;

	idx = 0;
	while (idx < str->length)
	{
		if (!is_whitespace(str->content[idx]))
			return (false);
		idx++;
	}
	return (true);
}

char	*ft_getenv(char **envp, const char *name)
{
	int		idx;
	size_t	len;

	len = ft_strlen(name);
	idx = 0;
	while (envp[idx])
	{
		if (ft_strncmp(envp[idx], name, len) == 0 && envp[idx][len] == '=')
			return (envp[idx] + len + 1);
		idx++;
	}
	return (NULL);
}

/// Returns ERR_OK or ERR_SYSTEM
static t_err	add_to_env(char ***envp, const char *name, const char *value)
{
	char	**old;
	char	**new;
	char	*name_suffix;
	size_t	len;

	old = *envp;
	len = 0;
	while (old[len])
		len++;
	new = malloc((len + 2) * sizeof(char *));
	if (!new)
		return (ERR_SYSTEM);
	ft_memcpy(new, old, len * sizeof(char *));
	name_suffix = ft_strjoin(name, "=");
	if (!name_suffix)
		return (ERR_SYSTEM);
	new[len] = ft_strjoin(name_suffix, value);
	if (!new[len])
		return (ERR_SYSTEM);
	new[len + 1] = NULL;
	free(name_suffix);
	free(old);
	*envp = new;
	return (ERR_OK);
}

t_err	do_all_vars(char ***envp, char *name_suffix, const char *value, size_t len)
{
	char	*name_new;
	size_t	idx;

	idx = 0;
	while ((*envp)[idx])
	{
		if (ft_strncmp((*envp)[idx], name_suffix, len) == 0)
		{
			name_new = ft_strjoin(name_suffix, value);
				if (!name_new)
				return (free(name_suffix), ERR_SYSTEM);
			free((*envp)[idx]);
			(*envp)[idx] = name_new;
			break ;
		}
		idx++;
	}
	return (ERR_OK);
}

/// Returns ERR_OK or ERR_SYSTEM
t_err	ft_setenv(char ***envp, const char *name, const char *value,
		int overwrite)
{
	char	*name_suffix;
	t_err	err;
	size_t	len;

	if (!value)
		ft_unsetenv(envp, name);
	if (!ft_getenv(*envp, name))
	{
		add_to_env(envp, name, value);
		return (ERR_OK);
	}
	if (!overwrite)
		return (ERR_OK);
	name_suffix = ft_strjoin(name, "=");
	if (!name_suffix)
		return (ERR_SYSTEM);
	len = ft_strlen(name_suffix);
	err = do_all_vars(envp, name_suffix, value, len);
	if (err == ERR_SYSTEM)
		return (ERR_SYSTEM);
	free(name_suffix);
	return (ERR_OK);
}

void	unsetenv_loop(size_t len, char *name_suffix, char **new, char **old)
{
	size_t	idx;
	size_t	n_skipped;

	idx = 0;
	n_skipped = 0;
	while (idx + n_skipped < len)
	{
		if (ft_strncmp(old[idx + n_skipped], name_suffix,
				ft_strlen(name_suffix)) == 0)
		{
			free(old[idx + n_skipped]);
			n_skipped++;
			continue ;
		}
		new[idx] = old[idx + n_skipped];
		idx++;
	}
	new[idx] = NULL;
}

/// Returns ERR_OK or ERR_SYSTEM
t_err	ft_unsetenv(char ***envp, const char *name)
{
	char	**old;
	char	**new;
	char	*name_suffix;
	size_t	len;

	if (!ft_getenv(*envp, name))
		return (ERR_OK);
	old = *envp;
	len = 0;
	while (old[len])
		len++;
	name_suffix = ft_strjoin(name, "=");
	if (!name_suffix)
		return (ERR_SYSTEM);
	new = malloc(len * sizeof(char *));
	if (!new)
		return (ERR_SYSTEM);
	unsetenv_loop(len, name_suffix, new, old);
	free(name_suffix);
	free(old);
	*envp = new;
	return (ERR_OK);
}

static size_t	size_t_max(size_t a, size_t b)
{
	if (a > b)
		return (a);
	else
		return (b);
}

/// errno will only be set on error
t_string	readline_lite(char *prompt)
{
	t_string	line;
	char		c;
	ssize_t		n_read;

	errno = 0;
	if (prompt)
		ft_putstr_fd(prompt, STDERR_FILENO);
	line = ft_string_new();
	if (!line.content)
		return (line);
	while (1)
	{
		n_read = read(STDIN_FILENO, &c, 1);
		if (n_read == -1)
			return (ft_string_destroy(&line), line);
		if (n_read == 0)
			return (ft_string_destroy(&line), line);
		if (c == '\n')
			return (line);
		if (!ft_string_ncat(&line, &c, 1))
			return (ft_string_destroy(&line), line);
	}
}

static char	*init_process_heredoc_delim(char *delim, char **quote, t_string *out)
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

/// Returns ERR_OK or ERR_SYSTEM
t_err	prompt_heredoc(int fd_out, char *delim, t_shell_data *shell_data)
{
	char		*no_expand;
	t_string	line;
	t_list		*expanded;
	t_list		*o_expanded;

	no_expand = process_heredoc_delim(delim);
	if (errno)
		return (ERR_SYSTEM);
	if (no_expand)
		delim = no_expand;
	while (1)
	{
		if (isatty(STDIN_FILENO))
		{
			errno = 0;
			if (USE_READLINE)
				line = ft_string_from(readline("> "));
			else
				line = readline_lite("> ");
		}
		else
			line = readline_lite(NULL);
		if (errno)
			return (ERR_SYSTEM);
		if (!line.content)
			break ;
		if (!ft_strncmp(line.content, delim, size_t_max(line.length,
					ft_strlen(delim))))
			break ;
		if (no_expand)
			ft_putstr_fd(line.content, fd_out);
		else
		{
			o_expanded = expand_arg(&line, shell_data, true);
			if (errno)
			{
				if (no_expand)
					free(delim);
				return (ERR_SYSTEM);
			}
			expanded = o_expanded;
			while (expanded && expanded->content)
			{
				ft_putstr_fd(((t_string *)expanded->content)->content, fd_out);
				expanded = expanded->next;
			}
			ft_lstclear(&o_expanded, lstclear_string);
		}
		ft_putstr_fd("\n", fd_out);
		ft_string_destroy(&line);
	}
	if (no_expand)
		free(delim);
	return (ERR_OK);
}

static bool	parse_options(char *arg, int *flags, char *options)
{
	int		new_flags;
	size_t	idx;
	size_t	opt_idx;

	new_flags = 0;
	idx = 1;
	while (arg[idx])
	{
		opt_idx = 0;
		while (options[opt_idx])
		{
			if (arg[idx] == options[opt_idx])
			{
				new_flags |= opt_idx + 1;
				break ;
			}
			opt_idx++;
		}
		if (!options[opt_idx])
			return (false);
		idx++;
	}
	*flags |= new_flags;
	return (true);
}

/// Returns ERR_OK or ERR_SYNTAX_ERROR
t_err	find_options(int *flags, char **args, size_t *idx, char *options)
{
	*flags = 0;
	while (args[*idx] && args[*idx][0] == '-')
	{
		if (!args[*idx][1])
			break ;
		if (args[*idx][1] == '-' || !options)
			return (ERR_SYNTAX_ERROR);
		if (!parse_options(args[*idx], flags, options))
			return (ERR_SYNTAX_ERROR);
		(*idx)++;
	}
	return (ERR_OK);
}

/// Returns ERR_OK or ERR_SYSTEM
t_err	lstadd_back_string(t_list **list, t_string str)
{
	t_string	*cell;
	t_list		*new;

	cell = (t_string *)malloc(sizeof(t_string));
	if (!cell)
		return (ERR_SYSTEM);
	*cell = str;
	new = ft_lstnew(cell);
	if (!new)
		return (ERR_SYSTEM);
	ft_lstadd_back(list, new);
	return (ERR_OK);
}

void	free_tab(void ***tab)
{
	size_t	idx;

	if (!*tab)
		return ;
	idx = 0;
	while ((*tab)[idx])
		free((*tab)[idx++]);
	free(*tab);
	*tab = NULL;
}
