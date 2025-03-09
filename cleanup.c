/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:23:44 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/07 16:23:27 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	clear_mem(t_data *data)
{
	if (data->path)
		free(data->path);
	if (data->paths)
		ft_free_split(data->paths);
}

void	close_fds(t_data *data)
{
	if (data->in != -1)
		close(data->in);
	if (data->out != -1)
		close(data->out);
	if (data->pipe[0] != -1)
		close(data->pipe[0]);
	if (data->pipe[1] != -1)
		close(data->pipe[1]);
}

void	ft_sys_error(t_data *data, int error)
{
	close_fds(data);
	clear_mem(data);
	if (error == PIPE)
		ft_error(PIPE, "pipe_error");
	else if (error == FORK)
		ft_error(FORK, "fork_error");
	else if (error == PID)
		ft_error(PID, "pid_error");
	else if (error == PATH)
		ft_error(PATH, "path_error");
	else if (error == CMD)
		ft_error(CMD, "cmd_error");

}

void	ft_error(int error, char *msg)
{
	if (error == PIPE || error == FORK || error == NOERROR
		|| error == PID || error == PATH || error == CMD)
	{
		perror(msg);
		return ;
	}
	ft_putstr_fd(msg, 2);
	ft_putstr_fd(": ", 2);
	if (error == NOPERMISSION || error == NOPERMISSIONPATH)
		ft_putendl_fd("Permission denied", 2);
	else if (error == NOFILE || error == NOPATH)
		ft_putendl_fd("No such file or directory", 2);
	else if (error == NOTFOUND || error == EMPTYCMD)
		ft_putendl_fd("Command not found", 2);
}
