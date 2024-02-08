#ifndef PIPEX_H
# define PIPEX_H
# include "libft/libft.h"
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <errno.h>
# include <stdbool.h>

# define MAX_ARGS 10
# define BUFFER_SIZE 1024

typedef struct s_pipex_data_m
{
	char	**cmd1_args;
	char	**cmd2_args;
	int		pipe_fds[2];
	char	**argv;
	char	**envp;
}	t_pipex_data_m;

typedef struct s_pipex_data
{
	int		n_cmds; // number of commands
	int		*pipefds; // array of pipe file descriptors
	int		n_pipes; // number of pipes
	bool	here_doc; // true if here_doc is used
	char	*limiter; // here_doc limiter
	char	*infile; // input file
	char	*outfile; // output file
	char	**argv; // array of command line arguments
	char	**envp; // array of environment variables
}	t_pipex_data;

int			validate_arguments(int argc);
char		**parse_command(char *cmd);
void		free_string_array(char ***array);
void		error_message(const char *message, int should_exit);
void		error_message_print(char *message, int should_exit);
void		cleanup(t_pipex_data_m *data);
char		**tokenize_path(const char *path);
int			ft_execvp(const char *file, char *const argv[], char *const envp[]);
void		redirect_first_command(t_pipex_data *pipeline);
void		redirect_last_command(t_pipex_data *pipeline);
void		redirect_intermediate_command(t_pipex_data *pipeline, int index);
void		redirect_here_doc(t_pipex_data *pipeline);
void		here_doc(char *limiter, int fd_out);
void		init_pipex_data(t_pipex_data *pipeline, \
	int argc, char **argv, char **envp);
void		cleanup_pipes_and_wait(t_pipex_data *pipeline);
void		create_pipes(int pipefds[], int n_pipes);
void		close_unused_pipe_ends(t_pipex_data *pipeline, int cmd_index);
void		close_all_pipe_fds(t_pipex_data *pipeline);

#endif
