/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:28:37 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/10 18:13:51 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdio.h>
# include <errno.h>

typedef struct s_data
{
	pid_t	pid1;
	pid_t	pid2;
	char	**cmd;
	char	**paths;
	char	*path;
	int		pipe[2];
	int		in;
	int		out;
	int		error_code;
	int		out_no_wr;
	int		in_no_r;
}	t_data;

typedef struct s_parse
{
	int		count;
	int		in_quote;
	char	quote_char;
	char	buffer[2000];
}	t_parse;

void	child_one(t_data *data, char *command, char **envp);
void	child_two(t_data *data, char *command, char **envp);
void	close_fds(t_data *data);
void	get_cmd_path(t_data *data, char *command, char **envp);
char	**parse_cmd(const char *str);
void	init_parse_data(t_parse *data);
void	init_data(t_data *data);
int		is_relative_path(t_data *data, char *cmd);
int		cmd_found(t_data *data);
void	ft_error_msg(t_data *data, char *arg, char *msg, int code);
void	clear_memory(t_data *data);
void	ft_sys_error(t_data *data, char *msg);

#endif
