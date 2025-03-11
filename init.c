/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:02:27 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/10 15:53:16 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	split_directories(t_data *data, char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		ft_error_msg(data, NULL, "Path not found", 127);
	data->paths = ft_split(envp[i] + 5, ':');
	if (!data->paths)
		ft_sys_error(data, "Memory allocation failed");
}

void	init_parse_data(t_parse *data)
{
	data->count = 0;
	data->in_quote = 0;
	data->quote_char = '\0';
}

void	init_data(t_data *data)
{
	data->paths = NULL;
	data->path = NULL;
	data->cmd = NULL;
	data->in = -1;
	data->out = -1;
	data->pipe[0] = -1;
	data->pipe[1] = -1;
	data->error_code = 0;
	data->out_no_wr = 0;
	data->in_no_r = 0;
	data->i = 1;
}
