#include "minishell.h"
#include <stdlib.h>
#include <errno.h>

t_err	expand_init(struct s_expand *expand)
{
	expand->out = NULL;
	expand->exp = ft_string_new();
	if (!expand->exp.content)
		return (ERR_SYSTEM);
	expand->del = '\0';
	expand->idx = 0;
	expand->has_empty_var = false;
	return (ERR_OK);
}

t_err	expand_redir(t_string *file_name, t_shell_data *shell_data)
{
	t_list	*lst;

	errno = 0;
	lst = expand_arg(file_name, shell_data, false);
	if (errno)
		return (ft_lstclear(&lst, lstclear_string), ERR_SYSTEM);
	if (!lst || !lst->content || ft_lstsize(lst) > 1)
		return (ft_lstclear(&lst, lstclear_string), ERR_COMMAND_FAILED);
	ft_string_destroy(file_name);
	ft_string_move((t_string *)lst->content, file_name);
	ft_lstclear(&lst, lstclear_string);
	return (ERR_OK);
}
