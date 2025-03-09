/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:22:32 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/07 18:01:49 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	is_space(char *cmd)
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

void	child_one(t_data *data, char *cmd, char **envp)
{
	data->error_status = NOERROR;
	if (!cmd || !ft_strlen(cmd) || is_space(cmd))
		ft_error(EMPTYCMD, cmd);
	else
	{
		data->cmd = parse_cmd(cmd);
		if (!data->cmd)
			ft_sys_error(data, CMD);
	}
	if (!is_relative_path(data, data->cmd[0]))
	{
		get_cmd_path(data, data->cmd[0], envp);
		if (data->path)
		{
			if (access(data->path, F_OK) == 0)
			{
				if (access(data->path, X_OK) != 0)
					data->error_status = NOFILE;
			}
			else
				data->error_status = NOFILE;
		}
	}
	dup2(data->in, 0);
	dup2(data->pipe[1], 1);
	close_fds(data);
	if (data->error_status != NOERROR
		|| execve(data->path, data->cmd, envp) == -1)
	{
		clear_mem(data);
		if (data->error_status == NOFILE || data->error_status == NOPERMISSION
		|| data->error_status == NOPERMISSIONPATH)
		{
			ft_error(data->error_status, cmd);
			exit(126);
		}
	}
}

void	child_two(t_data *data, char *cmd, char **envp)
{
	data->error_status = NOERROR;
	if (!cmd || !ft_strlen(cmd) || is_space(cmd))
		ft_error(EMPTYCMD, cmd);
	else
	{
		data->cmd = parse_cmd(cmd);
		if (!data->cmd)
			ft_sys_error(data, CMD);
	}
	if (!is_relative_path(data, data->cmd[0]))
	{
		get_cmd_path(data, data->cmd[0], envp);
		if (data->path)
		{
			if (access(data->path, F_OK) == 0)
			{
				if (access(data->path, X_OK) != 0)
					data->error_status = NOFILE;
			}
			else
				data->error_status = NOFILE;
		}
	}
	dup2(data->out, 1);
	dup2(data->pipe[0], 0);
	close_fds(data);
	if (data->error_status != NOERROR
		|| execve(data->path, data->cmd, envp) == -1)
	{
		clear_mem(data);
		if (data->error_status == NOFILE || data->error_status == NOPERMISSION
		|| data->error_status == NOPERMISSIONPATH)
		{
			ft_error(data->error_status, cmd);
			exit(126);
		}
		exit(127);
	}
}
