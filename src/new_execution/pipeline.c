#include "pipex.h"

void	create_pipes(int pipefds[], int n_pipes)
{
	int	i;

	i = 0;
	while (i < n_pipes)
	{
		if (pipe(pipefds + i * 2) < 0)
			error_message("pipe", 1);
		i++;
	}
}

void	close_unused_pipe_ends(t_pipex_data *pipeline, int cmd_index)
{
	if (cmd_index > 0)
		close(pipeline->pipefds[(cmd_index - 1) * 2]);
	if (cmd_index < pipeline->n_cmds - 1)
		close(pipeline->pipefds[cmd_index * 2 + 1]);
}

void	close_all_pipe_fds(t_pipex_data *pipeline)
{
	int	i;

	i = 0;
	while (i < 2 * pipeline->n_pipes)
		close(pipeline->pipefds[i++]);
}
