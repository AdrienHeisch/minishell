#include "minishell.h"
#include <stdlib.h>
#include <errno.h>

static char	*get_wildcard_pattern(char *s, size_t *len)
{
	char		del;
	t_string	pattern;

	pattern = ft_string_new();
	if (!pattern.content)
		return (NULL);
	del = '\0';
	*len = 0;
	while (s[*len])
	{
		if (!del && is_whitespace(s[*len]))
			break ;
		if (!del && (s[*len] == '\'' || s[*len] == '"'))
		{
			del = s[(*len)++];
			continue ;
		}
		if (s[*len] != del && !ft_string_ncat(&pattern, &s[*len], 1))
			return (NULL);
		if (s[(*len)++] == del)
			del = '\0';
	}
	if (ft_strnchr(s, '*', *len))
		return (pattern.content);
	return (ft_string_destroy(&pattern), NULL);
}

enum e_control_flow	catch_wildcard(t_string *arg, struct s_expand *expand)
{
	t_list	*wildcard;
	char	*pattern;
	size_t	pattern_len;

	pattern = get_wildcard_pattern(&arg->content[expand->idx], &pattern_len);
	if (expand->exp.length == 0 && pattern && expand->del == '\0')
	{
		expand->idx += pattern_len;
		errno = 0;
		wildcard = expand_wildcards(pattern);
		if (errno)
			return (free(pattern), CF_RETURN_ERR);
		if (!wildcard && !ft_string_cat(&expand->exp, pattern))
			return (free(pattern), CF_RETURN_ERR);
		while (wildcard)
			ft_lstadd_back(&expand->out, ft_lstpop_front(&wildcard));
		return (free(pattern), CF_CONTINUE);
	}
	return (free(pattern), CF_NONE);
}
static t_err				expand_var1(struct s_expand *expand, t_string var,
				char *pattern, size_t pattern_len);

t_err	expand_var(t_string *arg, t_shell_data *shell_data,
		struct s_expand *expand)
{
	t_string	var;
	t_string	pat;
	char		*pattern;
	size_t		len;

	len = 0;
	var = ft_string_new();
	if (!var.content)
		return (ERR_SYSTEM);
	while (expand->idx + len < arg->length
		&& is_var_name_char(arg->content[expand->idx + len]))
		len++;
	if (!ft_string_ncat(&var, &arg->content[expand->idx], len))
		return (ft_string_destroy(&var), ERR_SYSTEM);
	expand->idx += len;
	if (replace_var(&var, shell_data))
		return (ERR_SYSTEM);
	pat = ft_string_new();
	if (!pat.content || !ft_string_cat(&pat, var.content)
		|| !ft_string_cat(&pat, &arg->content[expand->idx]))
		return (ft_string_destroy(&var), ft_string_destroy(&pat), ERR_SYSTEM);
	pattern = get_wildcard_pattern(pat.content, &len);
	if (errno)
		return (ft_string_destroy(&var), ft_string_destroy(&pat), ERR_SYSTEM);
	return (ft_string_destroy(&pat), expand_var1(expand, var, pattern, len));
}

static t_err	expand_var1(struct s_expand *expand, t_string var,
		char *pattern, size_t pattern_len)
{
	t_list	*wildcard;

	if (pattern && expand->del == '\0')
	{
		expand->idx += pattern_len - var.length;
		errno = 0;
		wildcard = expand_wildcards(pattern);
		if (errno)
			return (ERR_SYSTEM);
		if (wildcard)
		{
			while (wildcard)
				ft_lstadd_back(&expand->out, ft_lstpop_front(&wildcard));
			return (ft_string_destroy(&var), free(pattern), ERR_OK);
		}
	}
	if (var.length == 0)
		expand->has_empty_var = true;
	if (var.length != 0 && !expand->del && split_var(var.content, &expand->exp,
			&expand->out))
		return (free(pattern), ERR_SYSTEM);
	if (var.length != 0 && expand->del && !ft_string_cat(&expand->exp,
			var.content))
		return (free(pattern), ERR_SYSTEM);
	return (free(pattern), ft_string_destroy(&var), ERR_OK);
}
