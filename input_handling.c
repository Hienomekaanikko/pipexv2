#include "pipex.h"

void	init(int argc, char **argv, t_data *data)
{
	if (argc != 5)
	{
		ft_putendl_fd("Usage: ./pipex infile cmd1 cmd2 outfile", 2);
		exit(1);
	}
	init_data(data);
	open_files(data, argc, argv);
}

void	check_access(t_data *data)
{
	if (access(data->path, F_OK) == 0)
	{
		if (access(data->path, X_OK) != 0)
			ft_error_msg(data, data->path, "Permission denied", 126);
	}
	else
		ft_error_msg(data, data->path, "Command not found", 127);
}

char	*handle_cmd(t_data *data, char *cmd, char **envp)
{
	char *cmd_path;

	cmd_path = NULL;
	if (!cmd || (ft_strlen(cmd) == 0) || is_space(cmd))
	{
		ft_error_msg(data, NULL, "Command not found", 127);
		return (NULL);
	}
	data->cmd = parse_cmd(cmd);
	if (!data->cmd)
		ft_sys_error(data, "Memory allocation failed");
	if (!is_relative_path(data, data->cmd[0]))
		get_cmd_path(data, data->cmd[0], envp);
	if (data->path)
		check_access(data);
	else
		ft_error_msg(data, NULL, "Command not found", 127);
	if (data->path)
	{
		cmd_path = ft_strdup(data->path);
		if (!cmd_path)
			ft_sys_error(data, "Memory allocation failed");
	}
	return (cmd_path);
}

void	open_files(t_data *data, int argc, char **argv)
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
	if (data->in == -1 && data->out == -1)
	{
		close_fds(data);
		exit(1);
	}
	if (pipe(data->pipe) < 0)
		ft_sys_error(data, "PIPE");
}
