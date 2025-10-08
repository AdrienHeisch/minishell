/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheisch <aheisch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 23:59:14 by aheisch           #+#    #+#             */
/*   Updated: 2025/10/08 23:59:14 by aheisch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

t_err	wait_all(int last_pid)

{
	int		status_location;
	t_err	exit_code;

	status_location = -1;
	exit_code = ERR_COMMAND_FAILED;
	if (last_pid != 0)
	{
		if (waitpid(last_pid, &status_location, 0) == -1)
			exit_code = ERR_SYSTEM;
		if (WIFEXITED(status_location))
			exit_code = WEXITSTATUS(status_location);
	}
	while (waitpid(0, &status_location, 0) > 0)
		;
	return (exit_code);
}
