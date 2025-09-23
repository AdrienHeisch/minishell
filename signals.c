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
#include <stdlib.h>
#include <unistd.h>

static void	handle_sigint(int sig)
{
	t_string	str;

	received_signal = sig;
	if (rl_readline_state & RL_STATE_READCMD)
	{
		str = ft_string_new();
		if (!str.content)
			exit(ERR_SYSTEM);
		if (!ft_string_cat(&str, rl_line_buffer))
			exit(ERR_SYSTEM);
		if (!ft_string_cat(&str, "^C"))
			exit(ERR_SYSTEM);
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
	(void)sig;
	if (rl_readline_state & RL_STATE_READCMD)
		rl_redisplay();
}

void	init_signals(void)
{
	struct sigaction	sig;

	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;
	sig.sa_handler = handle_sigint;
	sigaction(SIGINT, &sig, NULL);
	sig.sa_handler = handle_sigquit;
	sigaction(SIGQUIT, &sig, NULL);
}
