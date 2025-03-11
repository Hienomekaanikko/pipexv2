/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:22:32 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/10 17:30:06 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_one(t_data *data, char **envp)
{
	dup2(data->in, 0);
	dup2(data->pipe[1], 1);
	close_fds(data);
	if (execve(data->path, data->cmd, envp) == -1)
		clear_memory(data);
}

void	child_two(t_data *data, char **envp)
{
	dup2(data->out, 1);
	dup2(data->pipe[0], 0);
	close_fds(data);
	if (execve(data->path, data->cmd, envp) == -1)
		clear_memory(data);
}
