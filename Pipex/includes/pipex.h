/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galauren <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 03:22:30 by galauren          #+#    #+#             */
/*   Updated: 2025/06/27 01:30:39 by galauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
/*
**# include <stdio.h>
*/

char	*get_path_var(char **envp);
char	*join_path_cmd(char *dir, char *cmd);
char	*find_cmd_path(char *cmd, char **envp);
int		count_paths(const char *s);
char	*copy_dir(const char *start, int len);
char	**ft_split_path(const char *s);
int		count_args(const char *s);
char	**split_cmd(char *cmd);
void	exit_error(const char *msg, char **str, char ***tab);
int		open_infile(char *path);
int		open_outfile(char *path);
void	wait_all(void);

void	initialize_or_close_files(int close_them, int in, int out, int *pipe);

void	free_string(char **str);
void	free_tab(char ***tab);
void	free_choice(char **str, char ***tab);

#endif
