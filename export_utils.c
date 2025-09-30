#include "minishell.h"

int	allowed_char(int c)
{
	return (ft_isalnum(c) || c == '_');
}

/// Returns ERR_OK or ERR_COMMAND_FAILED
t_err	is_valid_var(char *name)
{
	if (ft_strlen(name) == 0)
		return (ERR_COMMAND_FAILED);
	if (ft_strchr(name, '!'))
		return (ERR_COMMAND_FAILED);
	if (ft_isdigit(name[0]))
		return (ERR_COMMAND_FAILED);
	if (!is_str_all(name, allowed_char))
		return (ERR_COMMAND_FAILED);
	return (ERR_OK);
}
