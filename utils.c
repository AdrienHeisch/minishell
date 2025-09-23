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
	char	*name_suffix;

	name_suffix = ft_strjoin(name, "=");
	if (!name_suffix)
		exit(ERR_ALLOC);
	len = ft_strlen(name_suffix);
	idx = 0;
	while (envp[idx])
	{
		if (ft_strncmp(envp[idx], name_suffix, len) == 0)
			return (free(name_suffix), envp[idx] + len);
		idx++;
	}
	free(name_suffix);
	return (NULL);
}

static void	add_to_env(char ***envp, const char *name, const char *value)
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
		exit(ERR_ALLOC);
	ft_memcpy(new, old, len * sizeof(char *));
	name_suffix = ft_strjoin(name, "=");
	if (!name_suffix)
		exit(ERR_ALLOC);
	new[len] = ft_strjoin(name_suffix, value);
	if (!new[len])
		exit(ERR_ALLOC);
	new[len + 1] = NULL;
	free(name_suffix);
	free(old);
	*envp = new;
}

void	ft_setenv(char ***envp, const char *name, const char *value,
		int overwrite)
{
	char	*name_suffix;
	size_t	len;
	size_t	idx;

	if (!value)
		ft_unsetenv(envp, name);
	if (!ft_getenv(*envp, name))
	{
		add_to_env(envp, name, value);
		return ;
	}
	if (!overwrite)
		return ;
	name_suffix = ft_strjoin(name, "=");
	if (!name_suffix)
		exit(ERR_ALLOC);
	len = ft_strlen(name_suffix);
	idx = 0;
	while ((*envp)[idx])
	{
		if (ft_strncmp((*envp)[idx], name_suffix, len) == 0)
		{
			free((*envp)[idx]);
			(*envp)[idx] = ft_strjoin(name_suffix, value);
			if (!(*envp)[idx])
				exit(ERR_SUCCESS);
			break ;
		}
		idx++;
	}
	free(name_suffix);
}

void	ft_unsetenv(char ***envp, const char *name)
{
	char	**old;
	char	**new;
	char	*name_suffix;
	size_t	len;
	size_t	idx;
	size_t	n_skipped;

	if (!ft_getenv(*envp, name))
		return ;
	old = *envp;
	len = 0;
	while (old[len])
		len++;
	name_suffix = ft_strjoin(name, "=");
	if (!name_suffix)
		exit(ERR_ALLOC);
	new = malloc(len * sizeof(char *));
	if (!new)
		exit(ERR_ALLOC);
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
	free(name_suffix);
	free(old);
	*envp = new;
}

static size_t	size_t_max(size_t a, size_t b)
{
	if (a > b)
		return (a);
	else
		return (b);
}

static char	*process_heredoc_delim(char *delim)
{
	char		*quote;
	char		*s;
	t_string	out;
	char		*rec;

	quote = ft_strchr(delim, '"');
	if (!quote)
		quote = ft_strchr(delim, '\'');
	if (!quote)
		return (NULL);
	out = ft_string_new();
	s = delim;
	while (*s)
	{
		if (*s != *quote)
		{
			if (!ft_string_ncat(&out, s, 1))
				exit(ERR_ALLOC);
		}
		s++;
	}
	rec = process_heredoc_delim(out.content);
	if (rec)
		return (rec);
	return (out.content);
}

t_string	readline_lite(void)
{
	t_string	line;
	char		c;
	ssize_t		n_read;

	line = ft_string_new();
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

void	prompt_heredoc(int fd_out, char *delim, t_shell_data *shell_data)
{
	char		*no_expand;
	t_string	line;
	t_list		*expanded;
	t_list		*o_expanded;

	if (!delim)
		return ;
	no_expand = process_heredoc_delim(delim);
	if (no_expand)
		delim = no_expand;
	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = ft_string_from(readline("> "));
		else
			line = readline_lite();
		if (!line.content)
			break ; // TODO error handling
		if (!ft_strncmp(line.content, delim, size_t_max(line.length,
					ft_strlen(delim))))
			break ;
		if (no_expand)
			ft_putstr_fd(line.content, fd_out);
		else
		{
			o_expanded = expand_arg(&line, shell_data, true);
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

int	find_options(int *flags, char **args, size_t *idx, char *options)
{
	*flags = 0;
	while (args[*idx] && args[*idx][0] == '-')
	{
		if (!args[*idx][1])
			break ;
		if (args[*idx][1] == '-' || !options)
			return (-1);
		if (!parse_options(args[*idx], flags, options))
			return (-1);
		(*idx)++;
	}
	return (0);
}

void	lstadd_back_string(t_list **list, t_string str)
{
	t_string	*cell;
	t_list		*new;

	cell = (t_string *)malloc(sizeof(t_string));
	if (!cell)
		exit(ERR_ALLOC);
	*cell = str;
	new = ft_lstnew(cell);
	if (!new)
		exit(ERR_ALLOC);
	ft_lstadd_back(list, new);
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
