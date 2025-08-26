/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 16:11:10 by aheisch           #+#    #+#             */
/*   Updated: 2025/08/25 16:11:10 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>
#include <unistd.h>

struct termios	set_terminal_attributes(void)
{
	struct termios	original_tio;
	struct termios	new_tio;

	tcgetattr(STDIN_FILENO, &original_tio);
	new_tio = original_tio;
	// new_tio.c_lflag &= ~ECHO;
	new_tio.c_lflag &= ~ECHOCTL;
	// new_tio.c_lflag &= ~ISIG;
	// new_tio.c_lflag &= ~ICANON;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
	return (original_tio);
}

void	restore_terminal_attributes(struct termios *original_tio)
{
	tcsetattr(STDIN_FILENO, TCSANOW, original_tio);
}
