/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:22:32 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/12 18:01:23 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	is_space(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (ft_isspace(cmd[i]))
			i++;
		else
			return (0);
	}
	return (1);
}

void	check_exitcode(t_data *data)
{
	if (data->out_no_wr == 1)
		exit(1);
	else
		exit(data->error_code);
}

void	child_one(t_data *data, char *path, char **cmd, char **envp)
{
	dup2(data->in, 0);
	dup2(data->pipe[1], 1);
	close_fds(data);
	execve(path, cmd, envp);
}

void	child_two(t_data *data, char *path, char **cmd, char **envp)
{
	dup2(data->out, 1);
	dup2(data->pipe[0], 0);
	close_fds(data);
	if (data->out_no_wr == 1 || execve(path, cmd, envp) == -1)
		check_exitcode(data);
}
