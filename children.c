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

void	check_exitcode(t_data *data)
{
	if (data->out_no_wr == 1)
		exit(1);
	if (data->error_code == 127)
		exit(127);
	if (data->error_code == 126)
		exit(126);
	if (data->error_code == 1)
		exit(1);
}

void	child_one(t_data *data, char *cmd, char **envp)
{
	if (!cmd || (ft_strlen(cmd) == 0) || is_space(cmd))
		ft_error_msg(data, NULL, "Command not found", 1);
	data->cmd = parse_cmd(cmd);
	if (!data->cmd)
		ft_sys_error(data, "Memory allocation failed");
	if (!is_relative_path(data, data->cmd[0]))
		get_cmd_path(data, data->cmd[0], envp);
	if (data->path)
	{
		if (access(data->path, F_OK) == 0)
		{
			if (access(data->path, X_OK) != 0)
				ft_error_msg(data, data->path, "Permission denied", 0);
		}
		else
			ft_error_msg(data, data->path, "Command not found", 0);
	}
	else
		ft_error_msg(data, NULL, "Command not found", 1);
	if (data->in_no_r == 0)
	{
		dup2(data->in, 0);
		dup2(data->pipe[1], 1);
	}
	close_fds(data);
	if (execve(data->path, data->cmd, envp) == -1)
		clear_memory(data);
}

void	child_two(t_data *data, char *cmd, char **envp)
{
	if (!cmd || (ft_strlen(cmd) == 0) || is_space(cmd))
		ft_error_msg(data, NULL, "Command not found", 127);
	data->cmd = parse_cmd(cmd);
	if (!data->cmd)
		ft_sys_error(data, "Memory allocation failed");
	if (!is_relative_path(data, data->cmd[0]))
		get_cmd_path(data, data->cmd[0], envp);
	if (data->path)
	{
		if (access(data->path, F_OK) == 0)
		{
			if (access(data->path, X_OK) != 0)
				ft_error_msg(data, data->path, "Permission denied", 126);
		}
		else
			ft_error_msg(data, data->path, "Command not found", 127);
	}
	else
		ft_error_msg(data, NULL, "Command not found", 127);
	dup2(data->out, 1);
	dup2(data->pipe[0], 0);
	close_fds(data);
	if (data->out_no_wr == 1 || data->error_code
		|| execve(data->path, data->cmd, envp) == -1)
	{
		clear_memory(data);
		check_exitcode(data);
	}
}
