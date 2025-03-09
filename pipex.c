/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:42:49 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/07 18:05:38 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	prep_env(t_data *data, int argc, char **argv)
{
	data->in = open(argv[1], O_RDONLY);
	if (data->in == -1 && access(argv[1], F_OK))
		ft_error(NOFILE, argv[1]);
	else if (data->in == -1 && !access(argv[1], F_OK))
		ft_error(NOPERMISSION, argv[1]);
	data->out = open(argv[argc - 1], O_TRUNC | O_CREAT | O_RDWR, 0644);
	if (data->out == -1 && access(argv[argc - 1], F_OK))
		ft_error(NOFILE, argv[argc - 1]);
	else if (data->out == -1 && !access(argv[argc - 1], F_OK))
		ft_error(NOPERMISSION, argv[argc - 1]);
	if (data->in == -1 && data->out == -1)
		return (0);
	if (pipe(data->pipe) < 0)
		ft_sys_error(data, PIPE);
	return (1);
}

static int	check_args(int argc)
{
	if (argc != 5)
	{
		ft_putendl_fd("Usage: ./pipex infile cmd1 cmd2 outfile", 2);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	int		status;

	init_data(&data);
	if (!check_args(argc))
		return (1);
	if (!prep_env(&data, argc, argv))
		return (1);
	data.pid1 = fork();
	if (data.pid1 == 0)
		child_one(&data, argv[2], envp);
	else if (data.pid1 == -1)
		ft_sys_error(&data, FORK);
	data.pid2 = fork();
	if (data.pid2 == 0)
		child_two(&data, argv[3], envp);
	else if (data.pid2 == -1)
		ft_sys_error(&data, FORK);
	close_fds(&data);
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			return (WEXITSTATUS(status));
	}
	return (0);
}
