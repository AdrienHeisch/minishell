/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exit_status.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 16:29:37 by aheisch           #+#    #+#             */
/*   Updated: 2025/10/16 16:29:37 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

int	handle_exit_status(int status_location)
{
	if (WIFEXITED(status_location))
		return (WEXITSTATUS(status_location));
	else if (WIFSIGNALED(status_location))
		return (128 + WTERMSIG(status_location));
	else
		return (ERR_SYSTEM);
}
