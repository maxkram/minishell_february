#include "pipex.h"

int	get_next_line(char **line)
{
	char	buf[2];
	int		ret;
	char	*temp;

	*line = malloc(1);
	if (!*line)
		error_message("Error: Malloc failed", 1);
	**line = '\0';
	while (1)
	{
		ret = read(STDIN_FILENO, buf, 1);
		if (ret <= 0)
			break ;
		buf[ret] = '\0';
		if (buf[0] == '\n')
			break ;
		temp = ft_strjoin(*line, buf);
		free(*line);
		*line = temp;
	}
	if (ret == -1)
		error_message("Error: Read failed", 1);
	return (ret);
}

void	here_doc(char *limiter, int write_end)
{
	char	*line;

	while (get_next_line(&line))
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
			break ;
		write(write_end, line, ft_strlen(line));
		write(write_end, "\n", 1);
		free(line);
	}
	free(line);
}
