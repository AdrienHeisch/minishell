#include "minishell.h"

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
