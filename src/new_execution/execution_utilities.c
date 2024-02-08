#include "pipex.h"

void	free_string_array(char ***array)
{
	int	i;

	i = 0;
	if (*array == NULL)
		return ;
	while ((*array)[i] != NULL)
	{
		free((*array)[i]);
		(*array)[i] = NULL;
		i++;
	}
	free(*array);
	*array = NULL;
}

void	error_message(const char *message, int should_exit)
{
	perror(message);
	if (should_exit)
		exit(1);
}

void	error_message_print(char *message, int should_exit)
{
	ft_putstr_fd(message, 2);
	if (should_exit)
		exit(1);
}

char	**tokenize_path(const char *path)
{
	char	**token_array;

	token_array = ft_split(path, ':');
	if (token_array == NULL)
	{
		error_message("ft_split", 0);
		return (NULL);
	}
	return (token_array);
}

void	cleanup_pipes_and_wait(t_pipex_data *pipeline)
{
	int	i;

	i = 0;
	while (i < 2 * pipeline->n_pipes)
	{
		close(pipeline->pipefds[i]);
		i++;
	}
	i = 0;
	while (i < pipeline->n_cmds)
	{
		wait(NULL);
		i++;
	}
}
