/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:42:49 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/10 18:22:51 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	processor(pid_t pid2)
{
	int		processes;
	int		status;
	int		exit_code;
	pid_t	pid;

	processes = 2;
	exit_code = 0;
	while (processes > 0)
	{
		pid = wait(&status);
		if (pid == pid2 && WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		processes--;
	}
	printf("%d\n", exit_code);
	return (exit_code);
}

static void	forker(t_data *data, char *cmd_path1, char *cmd_path2, char **envp)
{
	data->pid1 = fork();
	if (data->pid1 == 0)
	{
		data->path = cmd_path1;
		child_one(data, envp);
	}
	else if (data->pid1 == -1)
		ft_sys_error(data, "FORK");
	data->pid2 = fork();
	if (data->pid2 == 0)
	{
		data->path = cmd_path2;
		child_two(data, envp);
	}
	else if (data->pid2 == -1)
		ft_sys_error(data, "FORK");
}

int main(int argc, char **argv, char **envp)
{
	t_data data;
	char *cmd_path1;
	char *cmd_path2;

	init(argc, argv, &data);
	cmd_path1 = handle_cmd(&data, argv[2], envp);
	data.i = 2;
	cmd_path2 = handle_cmd(&data, argv[3], envp);
	forker(&data, cmd_path1, cmd_path2, envp);
	close_fds(&data);
	return (processor(data.pid2));
}
