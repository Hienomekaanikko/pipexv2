/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:42:49 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/12 18:01:44 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	prep_env(t_data *data, int argc, char **argv)
{
	data->in = open(argv[1], O_RDONLY);
	if (data->in == -1 && access(argv[1], F_OK) != 0)
		ft_error_msg(data, argv[1], "No such file or directory", 0);
	else if (data->in == -1 && access(argv[1], R_OK) != 0)
	{
		ft_error_msg(data, argv[1], "Permission denied", 0);
		data->in_no_r = 1;
	}
	data->out = open(argv[argc - 1], O_TRUNC | O_CREAT | O_RDWR, 0644);
	if (data->out == -1 && access(argv[argc - 1], F_OK) != 0)
		ft_error_msg(data, argv[argc - 1], "No such file or directory", 0);
	else if (data->out == -1 && access(argv[argc - 1], W_OK) != 0)
	{
		ft_error_msg(data, argv[argc - 1], "Permission denied", 0);
		data->out_no_wr = 1;
	}
}

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
	return (exit_code);
}

static void get_arguments(t_data *data, char **argv, char **envp)
{
	data->cmd1 = get_command(data, argv[2]);
	if (data->cmd1)
		data->path1 = get_command_path(data, data->cmd1, envp);
	data->curr = 2;
	data->error_code = 0;
	data->cmd2 = get_command(data, argv[3]);
	if (data->cmd2)
		data->path2 = get_command_path(data, data->cmd2, envp);
	if (pipe(data->pipe) < 0)
		ft_sys_error(data, "PIPE");
}

int main(int argc, char **argv, char **envp)
{
	t_data		data;

	init_data(&data);
	if (argc != 5)
	{
		ft_putendl_fd("Usage: ./pipex infile cmd1 cmd2 outfile", 2);
		exit(1);
	}
	prep_env(&data, argc, argv);
	get_arguments(&data, argv, envp);
	data.pid1 = fork();
	if (data.pid1 == 0)
		child_one(&data, data.path1, data.cmd1, envp);
	else if (data.pid1 == -1)
		ft_sys_error(&data, "FORK");
	data.pid2 = fork();
	if (data.pid2 == 0)
		child_two(&data, data.path2, data.cmd2, envp);
	else if (data.pid2 == -1)
		ft_sys_error(&data, "FORK");
	close_fds(&data);
	return (processor(data.pid2));
}
