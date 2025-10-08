#include "minishell.h"

/// Returns ERR_OK or ERR_SYSTEM
t_err	replace_var(t_string *var, t_shell_data *shell_data)
{
	t_string	var_name;
	char		*value;

	ft_string_move(var, &var_name);
	*var = ft_string_new();
	if (!var->content)
		return (ERR_SYSTEM);
	value = ft_getenv(shell_data->envp, var_name.content);
	if (!value)
		value = "";
	if (!ft_string_cat(var, value))
		return (ERR_SYSTEM);
	ft_string_destroy(&var_name);
	return (ERR_OK);
}

bool	is_var_name_start_char(char c)
{
	return (ft_isalpha(c) || c == '_');
}

bool	is_var_name_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/// Returns ERR_OK or ERR_SYSTEM
t_err	split_var(char *from, t_string *exp, t_list **out)
{
	char	*to;

	while (exp->length == 0 && is_whitespace(*from))
		from++;
	while (true)
	{
		to = ft_strchr(from, ' ');
		if (!to)
			to = ft_strchr(from, '\t');
		if (!to)
			to = ft_strchr(from, '\n');
		if (!to && !ft_string_cat(exp, from))
			return (ERR_SYSTEM);
		if (!to)
			return (ERR_OK);
		if (!ft_string_ncat(exp, from, to - from) || lstadd_back_string(out,
				*exp))
			return (ERR_SYSTEM);
		*exp = ft_string_new();
		if (!exp->content)
			return (ERR_SYSTEM);
		from = to;
		while (is_whitespace(*from))
			from++;
	}
}
