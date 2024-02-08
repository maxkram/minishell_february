#include "pipex.h"

void	init_pipex_data(t_pipex_data *pipeline, \
	int argc, char **argv, char **envp)
{
	if (pipeline->here_doc == true)
	{
		pipeline->n_cmds = argc - 4;
		pipeline->infile = NULL;
		pipeline->argv = argv + 3;
	}
	else
	{
		pipeline->n_cmds = argc - 3;
		pipeline->infile = argv[1];
		pipeline->argv = argv + 2;
	}
	pipeline->n_pipes = pipeline->n_cmds - 1;
	pipeline->pipefds = malloc(2 * pipeline->n_pipes * sizeof(int));
	if (!pipeline->pipefds)
		error_message("malloc", 1);
	pipeline->outfile = argv[argc - 1];
	pipeline->envp = envp;
}

void	execute_command(const char *cmd, t_pipex_data *pipeline)
{
	char	**args;

	args = ft_split(cmd, ' ');
	if (!args)
	{
		perror("ft_split");
		exit(EXIT_FAILURE);
	}
	if (ft_execvp(args[0], args, pipeline->envp) < 0)
	{
		perror("execvp");
		free_string_array(&args);
		free(pipeline->pipefds);
		exit(EXIT_FAILURE);
	}
	free(args);
}

void	create_child_process(t_pipex_data *pipeline, int cmd_index)
{
	int	j;

	if (cmd_index == 0 && pipeline->here_doc == true)
		redirect_here_doc(pipeline);
	else if (cmd_index == 0)
		redirect_first_command(pipeline);
	if (cmd_index == pipeline->n_cmds - 1)
		redirect_last_command(pipeline);
	if (cmd_index > 0)
	{
		close(pipeline->pipefds[(cmd_index - 1) * 2 + 1]);
		if (dup2(pipeline->pipefds[(cmd_index - 1) * 2], STDIN_FILENO) < 0)
			error_message("dup2 (stdin)", 1);
	}
	if (cmd_index < pipeline->n_cmds - 1)
	{
		close(pipeline->pipefds[cmd_index * 2]);
		if (dup2(pipeline->pipefds[cmd_index * 2 + 1], STDOUT_FILENO) < 0)
			error_message("dup2 (stdout)", 1);
	}
	j = 0;
	while (j < 2 * pipeline->n_pipes)
		close(pipeline->pipefds[j++]);
	execute_command(pipeline->argv[cmd_index], pipeline);
	error_message("execute_command", 1);
}

void	execute_pipeline(t_pipex_data *pipeline)
{
	int		i;
	pid_t	pid;

	create_pipes(pipeline->pipefds, pipeline->n_pipes);
	i = 0;
	while (i < pipeline->n_cmds)
	{
		pid = fork();
		if (pid == 0)
			create_child_process(pipeline, i);
		else if (pid < 0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else
			close_unused_pipe_ends(pipeline, i);
		i++;
	}
	close_all_pipe_fds(pipeline);
	cleanup_pipes_and_wait(pipeline);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex_data	pipeline;

	if (argc < 5)
		error_message_print("./pipex infile cmd1 cmd2 ... cmdn outfile\n", 1);
	if (ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1])) == 0)
	{
		if (argc != 6)
			error_message_print("./pipex here_doc LIMITER cmd1 cmd2 file\n", 1);
		pipeline.limiter = argv[2];
		pipeline.here_doc = true;
	}
	else
	{
		pipeline.limiter = NULL;
		pipeline.here_doc = false;
	}
	init_pipex_data(&pipeline, argc, argv, envp);
	execute_pipeline(&pipeline);
	free(pipeline.pipefds);
	return (EXIT_SUCCESS);
}
