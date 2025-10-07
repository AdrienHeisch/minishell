/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 19:18:03 by aheisch           #+#    #+#             */
/*   Updated: 2025/10/06 19:18:03 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
