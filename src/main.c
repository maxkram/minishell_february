#include "minishell.h"

//the function that aims to extract and split the directories listed in the PATH environment variable
/**
 * @changes:
 * 1. Temporarily changed PATH= to PATH=/home (I need this to test on linux)
*/
char	**path_getter(t_data *pnt, int i)
{
	char	**result;
	char	*found;

	while (pnt->env[++i])
	{
		found = ft_strstr(pnt->env[i], "PATH=");
		if (found != NULL)
			break ;
	}
	if (pnt->env[i] == NULL)
		return (NULL);
	result = ft_split_fd(found, ':');
	if (!result)
	{
		free(result);
		return (error_out(pnt, "ft_split", 1), NULL);
	}
	return (result);
}

//function in the minishell project of Ecole 42 is responsible for initializing the environment data in the t_data structure. It takes as input the t_data pointer (data) and an array of strings (env) representing the environment variables. (130 == CTRL+C)

void env_init(t_data *data, char **env)
{
	int i;

	i = 0;
	while(env[i])
		i++;
	data->env = ft_calloc(i + 1, sizeof(char *));
	i = 0;
	while(env[i])
	{
		data->env[i] = ft_strdup_fd(env[i]);
		i++;
	}
}



int	main(int argc, char *argv[], char **env_p)
{
	t_data	pnt;

	(void)argv;
	(void)argc;
	// if (argc != 1)
	// 	return (ft_putstr("Error: too many arguments\n", 2));
	init_pntr(&pnt, env_p);
	while (1)
	{
		set_mode(&pnt, INTERACT);
		pnt.input = readline("minishell$ ");
		set_mode(&pnt, NON_INTERACT);
		if (global_signal == 1 && global_signal--)
			pnt.code_exit = 130;
		if (!pnt.input)
			built_exit(&pnt, NULL);
		if (pnt.input[0] != '\0')
			add_history(pnt.input);
		pnt.path = path_getter(&pnt, -1);
		if (tokener(&pnt) == 0 && extender(&pnt) == 0 && parser(&pnt) == 0)
		{
			alt_exec_main(&pnt);
			// execute_pipeline(&pnt);
		}
		pntr_cleaning(&pnt);
	}
}

/************************************ NEW EXECUTION **********************************************/

void	execute_pipeline(t_data *pnt)
{
	int		i;
	pid_t	pid;


	printf("pnt->n_pipes: %d\n", pnt->n_pipes);
	printf("pnt->pipefds: %d\n", *pnt->pipefds);
	pnt->pipefds = malloc(2 * pnt->n_pipes * sizeof(int));
	if (!pnt->pipefds)
		error_out(pnt, "malloc", 1);
	create_pipes(pnt);
	i = 0;
	while (i < pnt->n_cmds)
	{
		pid = fork();
		if (pid == 0)
			create_child_process(pnt, i);
		else if (pid < 0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
	// 	else
	// 		close_unused_pipe_ends(pipeline, i);
	// 	i++;
	}
	// close_all_pipe_fds(pipeline);
	// cleanup_pipes_and_wait(pipeline);
}

void	create_pipes(t_data *pnt)
{
	int	i;

	i = 0;
	while (i < pnt->n_pipes)
	{
		if (pipe(pnt->pipefds + i * 2) < 0)
			error_out(pnt, "pipe", 1);
		i++;
	}
}

void	create_child_process(t_data *pnt, int cmd_index)
{
	int	j;

	// if (cmd_index == 0 && pnt->here_doc == true)
	// 	redirect_here_doc(pnt);
	if (cmd_index == 0)
		redirect_first_command(pnt);
	if (cmd_index == pnt->n_cmds - 1)
		redirect_last_command(pnt);
	if (cmd_index > 0)
	{
		close(pnt->pipefds[(cmd_index - 1) * 2 + 1]);
		if (dup2(pnt->pipefds[(cmd_index - 1) * 2], STDIN_FILENO) < 0)
			error_message("dup2 (stdin)", 1);
	}
	if (cmd_index < pnt->n_cmds - 1)
	{
		close(pnt->pipefds[cmd_index * 2]);
		if (dup2(pnt->pipefds[cmd_index * 2 + 1], STDOUT_FILENO) < 0)
			error_message("dup2 (stdout)", 1);
	}
	j = 0;
	while (j < 2 * pnt->n_pipes)
		close(pnt->pipefds[j++]);
	// execute_command(pnt->argv[cmd_index], pnt);
	error_message("execute_command", 1);
}

void	redirect_here_doc(t_data *pnt)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) < 0)
		error_message("pipe", 1);
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		here_doc(pnt->limiter, fd[1]);
		close(fd[1]);
		free(pnt->pipefds);
		exit(EXIT_SUCCESS);
	}
	else if (pid < 0)
		error_message("fork", 1);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
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

void	redirect_first_command(t_data *pnt)
{
	int	fd_in;

	if (pnt->limiter)
		return ;
	fd_in = open(pnt->infile, O_RDONLY, 0644);
	if (fd_in < 0)
		error_message("open (infile)", 1);
	dup2(fd_in, STDIN_FILENO);
	close(fd_in);
}

void	redirect_last_command(t_data *pnt)
{
	int	fd_out;

	if (pnt->here_doc)
	{
		fd_out = open(pnt->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd_out < 0)
			error_message("open (outfile)", 1);
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	else
	{
		fd_out = open(pnt->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd_out < 0)
			error_message("open (outfile)", EXIT_FAILURE);
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
}
