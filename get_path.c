/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:25:43 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/10 17:57:42 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*build_path(t_data *data, char *cmd, int i)
{
	char	*path;
	char	*full_path;

	path = ft_strjoin(data->paths[i], "/");
	if (!path)
		ft_sys_error(data, "Memory allocation failed");
	full_path = ft_strjoin(path, cmd);
	free(path);
	if (!full_path)
		ft_sys_error(data, "Memory allocation failed");

	return (full_path);
}

int	is_relative_path(t_data *data, char *cmd)
{
	if (access(cmd, F_OK) == 0 && access(cmd, X_OK) == 0)
	{
		data->path = cmd;
		return (1);
	}
	data->path = NULL;
	return (0);
}

static void	add_absolute_path(t_data *data, char *cmd)
{
	data->path = ft_strdup(cmd);
	if (!data->path)
		ft_sys_error(data, "Memory allocation failed");
	if (access(data->path, F_OK) != 0)
	{
		ft_error_msg(data->path, "No such file or directory");
		if (data->i == 2 && data->in_error == 0)
			data->out_error = 127;
		if (data->i == 1 && data->in_error == 0)
			data->in_error = 1;
	}
}

void	get_cmd_path(t_data *data, char *cmd, char **envp)
{
	int	i;

	i = 0;
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		add_absolute_path(data, cmd);
		return ;
	}
	split_directories(data, envp);
	while (data->paths[i])
	{
		data->path = build_path(data, cmd, i);
		if (access(data->path, F_OK) == 0 && access(data->path, X_OK) == 0)
		{
			ft_free_split(data->paths);
			data->paths = NULL;
			return ;
		}
		free(data->path);
		data->path = NULL;
		i++;
	}
}
