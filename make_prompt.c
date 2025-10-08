/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_prompt.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 23:30:55 by aheisch           #+#    #+#             */
/*   Updated: 2025/09/29 23:30:55 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

/// Returns ERR_OK, ERR_COMMAND_FAILED or ERR_SYSTEM
static t_err	add_hostname_and_cwd(t_string *prompt, char **envp)
{
	char	*s;
	char	*h;

	s = ft_getenv(envp, "HOSTNAME");
	if (!s)
		return (ERR_COMMAND_FAILED);
	if (!ft_string_cat(prompt, s))
		return (ERR_SYSTEM);
	if (!ft_string_cat(prompt, ":"))
		return (ERR_SYSTEM);
	s = ft_getenv(envp, "PWD");
	if (!s)
		return (ERR_COMMAND_FAILED);
	h = ft_getenv(envp, "HOME");
	if (h && !ft_strncmp(s, h, ft_strlen(h)))
	{
		if (!ft_string_cat(prompt, "~"))
			return (ERR_SYSTEM);
		if (!ft_string_cat(prompt, s + ft_strlen(h)))
			return (ERR_SYSTEM);
	}
	else if (!ft_string_cat(prompt, s))
		return (ERR_SYSTEM);
	return (ERR_OK);
}

/// Will return null and errno will be set on error
char	*make_prompt(char **envp)
{
	t_string	prompt;
	char		*s;
	t_err		err;

	prompt = ft_string_new();
	if (!prompt.content || !ft_string_cat(&prompt, "\001\033[36m\002"))
		return (ft_string_destroy(&prompt), NULL);
	s = ft_getenv(envp, "USER");
	if (!s)
		return (ft_string_destroy(&prompt), "$ ");
	if (!ft_string_cat(&prompt, s) || !ft_string_cat(&prompt, "@"))
		return (ft_string_destroy(&prompt), NULL);
	err = add_hostname_and_cwd(&prompt, envp);
	if (err == ERR_SYSTEM)
		return (ft_string_destroy(&prompt), NULL);
	if (err)
		return (ft_string_destroy(&prompt), "$ ");
	if (!ft_string_cat(&prompt, "\001\033[0m\002")
		|| !ft_string_cat(&prompt, "$ "))
		return (ft_string_destroy(&prompt), NULL);
	return (prompt.content);
}
