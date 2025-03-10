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

static void	add_cmd_to_path(t_data *data, char *cmd, int i)
{
	char	*path;

	path = ft_strjoin(data->paths[i], "/");
	if (!path)
		ft_sys_error(data, "Memory allocation failed");
	data->path = ft_strjoin(path, cmd);
	free(path);
	path = NULL;
	if (!data->path)
		ft_sys_error(data, "Memory allocation failed");
}

static void	split_directories(t_data *data, char **envp)
{
	int	i;

	i = 0;
	if (!envp[0])
		ft_error_msg(data, NULL, "Path not found", 127);
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		ft_error_msg(data, NULL, "Path not found", 127);
	data->paths = ft_split(envp[i] + 5, ':');
	if (!data->paths)
		ft_sys_error(data, "Memory allocation failed");
}

static void	add_absolute_path(t_data *data, char *cmd)
{
	data->path = ft_strdup(cmd);
	if (!data->path)
		ft_sys_error(data, "Memory allocation failed");
	if (access(data->path, F_OK) != 0)
		ft_error_msg(data, data->path, "No such file or directory", 127);
}

void	get_cmd_path(t_data *data, char *cmd, char **envp)
{
	int	i;

	if (cmd[0] == '/' || cmd[0] == '.')
	{
		add_absolute_path(data, cmd);
		return ;
	}
	split_directories(data, envp);
	i = 0;
	while (data->paths[i])
	{
		add_cmd_to_path(data, cmd, i);
		if (cmd_found(data))
			return ;
		i++;
		free(data->path);
		data->path = NULL;
	}
}
