/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 19:22:59 by aheisch           #+#    #+#             */
/*   Updated: 2025/10/20 19:22:59 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>

int	g_heredoc_exit_flag = false;

static void	handle_sigint(int sig)
{
	(void)sig;
	rl_done = true;
	g_heredoc_exit_flag = true;
}

static int	event_hook(void)
{
	return (ERR_OK);
}

/// Returns ERR_OK or ERR_SYSTEM
t_err	init_signals_heredoc(void)
{
	rl_event_hook = event_hook;
	if (signal(SIGINT, handle_sigint) == SIG_ERR)
		return (ERR_SYSTEM);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		return (ERR_SYSTEM);
	return (ERR_OK);
}
