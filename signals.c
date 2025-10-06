/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:23:54 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/25 14:23:54 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>

static void	handle_sigint(int sig)
{
	t_string	str;

	g_received_signal = sig;
	if (USE_READLINE && rl_readline_state & RL_STATE_READCMD)
	{
		str = ft_string_new();
		if (!str.content)
			return (print_error(), (void)0);
		if (!ft_string_cat(&str, rl_line_buffer))
			return (print_error(), (void)0);
		if (!ft_string_cat(&str, "^C"))
			return (print_error(), (void)0);
		rl_replace_line(str.content, false);
		rl_redisplay();
		rl_on_new_line();
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_replace_line("", false);
		rl_redisplay();
	}
}

static void	handle_sigquit(int sig)
{
	g_received_signal = sig;
	(void)sig;
	if (rl_readline_state & RL_STATE_READCMD)
		rl_redisplay();
}

/// Returns ERR_OK or ERR_SYSTEM
t_err	init_signals(void)
{
	struct sigaction	sig;

	if (sigemptyset(&sig.sa_mask))
		return (ERR_SYSTEM);
	sig.sa_flags = 0;
	sig.sa_handler = handle_sigint;
	if (sigaction(SIGINT, &sig, NULL))
		return (ERR_SYSTEM);
	sig.sa_handler = handle_sigquit;
	if (sigaction(SIGQUIT, &sig, NULL))
		return (ERR_SYSTEM);
	return (ERR_OK);
}
