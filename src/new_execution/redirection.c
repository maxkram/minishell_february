#include "pipex.h"

void	redirect_first_command(t_pipex_data *pipeline)
{
	int	fd_in;

	if (pipeline->limiter)
		return ;
	fd_in = open(pipeline->infile, O_RDONLY, 0644);
	if (fd_in < 0)
		error_message("open (infile)", 1);
	dup2(fd_in, STDIN_FILENO);
	close(fd_in);
}

void	redirect_last_command(t_pipex_data *pipeline)
{
	int	fd_out;

	if (pipeline->here_doc)
	{
		fd_out = open(pipeline->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd_out < 0)
			error_message("open (outfile)", 1);
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	else
	{
		fd_out = open(pipeline->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd_out < 0)
			error_message("open (outfile)", EXIT_FAILURE);
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
}

void	redirect_intermediate_command(t_pipex_data *pipeline, int index)
{
	if (dup2(pipeline->pipefds[(index - 1) * 2], STDIN_FILENO) < 0)
		error_message("dup2 (stdin)", 1);
	if (dup2(pipeline->pipefds[index * 2 + 1], STDOUT_FILENO) < 0)
		error_message("dup2 (stdout)", 1);
}

void	redirect_here_doc(t_pipex_data *pipeline)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) < 0)
		error_message("pipe", 1);
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		here_doc(pipeline->limiter, fd[1]);
		close(fd[1]);
		free(pipeline->pipefds);
		exit(EXIT_SUCCESS);
	}
	else if (pid < 0)
		error_message("fork", 1);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
}
