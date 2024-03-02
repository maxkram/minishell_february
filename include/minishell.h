/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:43:52 by hezhukov          #+#    #+#             */
/*   Updated: 2024/03/02 15:03:37 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <../vendor/readline/include/readline/readline.h>
# include <../vendor/readline/include/readline/history.h>

# include <string.h>
# include <fcntl.h>
# include <stddef.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <sys/wait.h>
# include <unistd.h>
# include <errno.h>
# include <dirent.h>
# include <limits.h>
# include <stdbool.h>

//==================VALUES===================//
# define DEBUG_ON 0
# define TRUE 1
# define FALSE 0
# define YES 1
# define NO 0
# define PROMPT_NAME "\033[35m\033[1mminiSHELL \033[0m\033[1m→ \033[0m"
# define EXPORT_PREFIX "declare -x "
# define WRITE_END 1
# define READ_END 0
# define ERROR_TOO_MANY_ARGS "Error: too many arguments\n"
# define PROMPT "\033[31m\033[1mminishell$ \033[0m"
# define EXIT_CODE_SIGNAL 130
# define NON_INTERACTIVE_MODE 0
# define INTERACTIVE_MODE 1
//==================ERRORS===================//
# define ERR_MAIN "\033[31m\033[1mERROR > \033[0m"
# define ERR_EXP "\033[37m\033[1mREASON\033[0m"
# define ERR_FORK "Forked up!\n"
# define ERR_ALLOC "Allocation error\n"
# define ERR_PIPE "Couldn't open the pipe\n"
# define ERR_DUP "Dup error\n"
# define ERR_CD_ARGS "Too much arguments for CD\n"
# define ERR_CD "Invalid name or path\n"
# define ERR_EXPORT "Invalid identifier for export\n"
# define ERR_UNSET "Invalid indentifier for unset\n"
# define ERR_WRITE "Write error\n"
# define ERROR -1
//==================Structs===================//

extern int	g_global_signal;

typedef enum s_type_token
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	REDIRECT_MULTILINE,
	DQUOTE,
	SQUOTE
}	t_type_token;

typedef enum s_set_mode
{
	INTERACT,
	NON_INTERACT,
	CHILD,
	MULTILINE
}	t_set_mode;

typedef struct s_token
{
	char			*value;
	t_type_token	type;
	int				no_space;
}	t_token;

/**
 * @brief Command table data structure
 * @param cmd - command name
 * @param args - arguments
 * @param in_fd - input file descriptor
 * @param out_fd - output file descriptor
 * @param redirections - redirections
 * @param file_in - input file
 * @param file_out - output file
 * @param num_redirections - number of redirections
 * @param num_args - number of arguments
 * @param is_child_process - is child process
 * @param last_multiline - last multiline
 * @param pid - process id
 * @param num_pipes - number of pipes
 * @param mode - mode
 *
*/
typedef struct s_tab_cmd
{
	char	*cmd;
	char	**args;
	int		in_fd;
	int		out_fd;
	t_token	*redirections;
	int		file_in;
	int		file_out;
	int		num_redirections;
	int		num_args;
	int		is_child_process;
	char	*last_multiline;
	pid_t	pid;
}	t_tab_cmd;

/**
 * @brief Data structure
 * @param env - environment variables (env)
 * @param path - path to executables (binaries)
 * @param input - input string from user prompt or file
 * @param tokens - tokens (broken pieces of whole input)
 * @param count_token - count token (counter how many tokens in the input)
 * @param max_token - max token (maximum of tokens (memory allocation purpose))
 * @param cmdt - command table (the link to a command table data structure)
 * @param cmdt_count - command table count (how many of child data structs)
 * @param code_exit - exit code (to keep status of exit (2, 1, 0))
 * @param fd_before - file descriptor before (file descriptor in the beginning)
 * @param first_stdin - first stdin
 * @param first_stdout - first stdout
 * @param n_pipes - number of pipes (to keep number of pipes)
 * @param mode - mode (set mode)
*/
typedef struct s_data
{
	char		**env;
	char		**path;
	char		*input;
	t_token		*tokens;
	int			count_token;
	int			max_token;
	t_tab_cmd	*cmdt;
	int			cmdt_count;
	int			code_exit;
	int			fd_before;
	int			first_stdin;
	int			first_stdout;
	int			n_pipes;
	t_set_mode	mode;
	int			fd_pipe[2];
}	t_data;

void	prompt_create(t_data *data);
void	build_pwd(t_data *pnt);
int		if_builtin(t_tab_cmd *cmd_tab);
void	execute_builtin(t_data *pntr, t_tab_cmd *cmd_tab, int i);
void	exec_main(t_data *data);
void	execution(t_data *pntr);
char	*ft_get_env(char *str, char **env);
char	*cmd_fullpath(t_data *data, char *cmd);
void	ft_dup2(int fd, int std);
int		change_fd_input_output(t_data *pntr, t_tab_cmd *tab_cmd, int i);
int		find_path(t_data *pntr, t_tab_cmd *tab_cmd);
int		built_cd(t_data *pnt, t_tab_cmd *tab_cmd);
int		make_var(t_data *pnt, char *var_name, char *value);
void	built_exit(t_data *pntr, t_tab_cmd *cmd_tab);
void	built_echo(t_data *pnt, t_tab_cmd *tab_cmd);
void	built_unset(t_data *pnt, t_tab_cmd *tab_cmd);
void	index_sort(char **arr, int str_count, int *index);
int		built_env(t_data *data);
int		var_in_env(char *str);
void	print_export(char **env, int fd);
int		built_export(t_data *data, t_tab_cmd *cmd);
int		parser(t_data *pointer);
int		redirections_fill(t_data *pointer, int i, int j);
int		words_merging(t_data *pointer);
int		count_pipes(t_data *pointer);
int		new_tokens_count(t_data *pointer);
int		token_copy(t_data *pointer, t_token *tokens_new, int *i, int *j);
void	clean_double_pointer(char **pnt);
int		word_filling(t_data *pntr, char const *str);
int		reallocate_tokens_if_max(t_data *pntr, int max_token);
int		word_filling(t_data *pntr, char const *s);
int		filling_quotes(t_data *pntr, char const *str, char tmp);
int		tokener(t_data *pntr);
int		extender(t_data *pntr);
int		dollar_replacement(char *string, \
	char **value, t_data *pntr, int exception);
char	*value_of_variable(t_data *pntr, char *i);
int		variable_index(char **env, char *n);
int		length_of_variable(const char *n);
int		if_has(const char *n, char c);
int		substring_concatenation(char *string, char **pntr);
int		pipelines_redirect(t_data *pntr, int i);
int		create_heredoc(t_data *pntr, t_tab_cmd *tab_cmd, int i);
void	set_mode(t_data *pntr, t_set_mode mode);
int		find_exec(t_data *pntr, t_tab_cmd *cmd_tab);
void	fd_cleaning(t_data *pntr, t_tab_cmd *tab_cmd, int i);
void	total_clean(t_data *pntr);
void	pntr_cleaning(t_data *pntr);
void	init_pnt(t_data *pntr, char **env);
void	free_string_array(char **pnt);
void	fd_exit(t_data *pntr, int code_exit);
int		ft_putstr_int(char *s, int fd);
int		increase_shlvl(t_data *pntr);
int		create_env_var(t_data *pntr, char *key, char *keep);
void	cmdt_cleaning(t_data *pntr);
void	pntr_cleaning(t_data *pnt);
void	fd_cleaning(t_data *pntr, t_tab_cmd *tab_cmd, int i);
void	total_clean(t_data *pntr);
int		clean_tokens(t_token *tokens, int max, t_data *pnt);
void	cleanup_tokens(t_data *data);
void	cleanup_commands(t_data *pnt);
t_data	*get_data(void);
void	ft_error(char *str);
void	my_error(char *str);
void	safe_free(void **ptr);
void	error_in_syntax(int type, t_data *pntr);
void	free_all(t_data *data);
void	free_ptr(void *ptr);
void	free_str_tab(char **tab);
int		check_exception(t_data *pnt, int i);
int		variable_index(char **env, char *n);
int		substring_beginning(char *word, char *start);
int		path_searching(t_data *data, t_tab_cmd *tab_cmd);
int		fill_redirection(t_data *pnt, char const *str);
int		realloc_and_check(t_data *pnt);
int		handle_pipe(t_data *pnt);
int		handle_quotes(t_data *pnt, int *i);
int		handle_word(t_data *pnt, int *i);
int		clean_token_array(t_token *array_tokens, \
	int max_number_token, t_data *pnt);
int		reallocate_tokens_if_max(t_data *pnt, int max_token);
int		parse_and_fill_command(t_data *data, \
	int command_index, int token_index);
int		input_output_redirect(t_data *pnt, t_tab_cmd *tab_cmd);
int		setup_pipes(t_data *pnt);
void	execute_command(t_data *pnt, int i);
void	handle_redirection(int fd, int std_channel);
void	close_pipe_end(int *fd_pipe, int end);
void	cleanup_heredoc(t_data *pntr, int i);
int		in_to_fd(t_data *pnt, int fd, char *delimiter);
char	**path_getter(t_data *pnt, int i);
void	env_init(t_data *pnt, char **env);
int		check_arguments(t_type_token type);
int		check_if_redirection(t_type_token type);
void	free_ptr(void *ptr);
int		error_out(t_data *pnt, char *s, int n);
int		env_var_count(char **env);
int		print_env_vars(t_data *data);
void	set_error_and_code(char *cmd_arg, int *code_exit);

#endif
