#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <../vendor/readline/include/readline/readline.h> //i don't know why but it works on my mac's account
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

extern int	global_signal;

typedef	enum s_type_token //names for token like '>', '<<', '|', '""'
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	REDIRECT_MULTILINE, //heredoc @todo rename to heredoc
	DQUOTE,
	SQUOTE
}	t_type_token;

typedef	enum s_set_mode //names for here-doc modes
{
	INTERACT,
	NON_INTERACT,
	CHILD,
	MULTILINE
}	t_set_mode;

typedef	struct s_token //for tokenizer
{
	char			*value;
	t_type_token	type;
	int				no_space; // This is a flag to check if it's a space. If it's a space should be processed differently
}	t_token;

/**
 * @brief Command table data structure
 *
*/
typedef struct s_tab_cmd //data struct for keeping status when we operate with files
{
	char	*cmd; //to keep results of search
	char	**args; //counts number of argc in main
	int		in_fd; //file descriptors
	int		out_fd; //file descriptors
	t_token	*redirections;
	int		file_in; //file descriptors
	int		file_out; //file descriptors
	int		num_redirections; // to count number of ">>"s
	int		num_args; //number of arguments
	int		is_child_process; //boolean if the process the result of fork() or dup2()
	char	*last_multiline; //here-doc status
	pid_t	pid; //pid
}	t_tab_cmd;

typedef struct s_data
{
	char		**env; //to keep the 3rd argument from main : env
	char		**path; // keep PATH, used for search
	char		*input; // for everything from input
	t_token		*tokens; // keep broken pieces of whole input
	int			count_token; //just counter how many tokens in the input
	int			max_token; //maximum of tokens (memory allocation purpose)
	t_tab_cmd	*cmdt; //the link to another data structure
	int			cmdt_count; // counter how many of child data structs
	// char		*user_prompt;  // legacy code
	int			code_exit; // to keep status of exit (2, 1, 0)
	// int		exit_promt; // legacy code
	int			fd_before; // file descriptor in the beginning
	int			first_stdin; //when we create file decriptors we need STDIN_FILENO
	int			first_stdout; // like previous for STDOUT_FILENO
	int			n_pipes; // to keep number of pipes
	t_set_mode	mode;
}	t_data;

//==================[t_utils.c]===================//

//
//==================[prompt.c]===================//
void	prompt_create(t_data *data);
//
//==================[builtin.c]===================//
void	build_pwd(t_data *pnt);
int		if_builtin(t_tab_cmd *cmd_tab);
void    shoot_builtin(t_data *pntr, t_tab_cmd *cmd_tab, int i, int *pipe_fd);
//
//==================[exec.c]===================//
void	exec_main(t_data *data);
void	alt_exec_main(t_data *pntr);
//
//==================[exec_utils.c]===================//
char	*ft_get_env(char *str, char **env);
char	*cmd_fullpath(t_data *data, char *cmd);
void	ft_dup2(int fd, int std);
int		change_fd_input_output(t_data *pntr, t_tab_cmd *tab_cmd, int *fd, int i);
int		find_path(t_data *pntr, t_tab_cmd *tab_cmd);
//
//==================[b_cd.c]===================//
int		built_cd(t_data *pnt, t_tab_cmd *tab_cmd);
int		make_var(t_data *pnt, char *var_name, char *value);
//
//==================[b_exit.c]===================//
void	built_exit(t_data *pntr, t_tab_cmd *cmd_tab);
//
//==================[b_echo.c]===================//
void	built_echo(t_data *pnt, t_tab_cmd *tab_cmd);
//
//==================[b_unset.c]===================//
void	built_unset(t_data *pnt, t_tab_cmd *tab_cmd);
//
//==================[b_env.c]===================//
void	index_sort(char **arr, int str_count, int *index);
// void	print_env(char **env, int fd);
int		built_env(t_data *data);
//
//==================[b_export.c]===================//
int		var_in_env(char *str);
void	print_export(char **env, int fd);
int		built_export(t_data *data, t_tab_cmd *cmd);
//
//==================[parser.c]===================//
int		parser(t_data *pointer);
//
//==================[parser_func.c]===================//
int 	redirections_fill(t_data *pointer, int i, int j);
int		args_cmd_fill(t_data *pointer, int i, int j);
int		words_merging(t_data *pointer);
//
//==================[parser_utils.c]===================//
int		check_if_redirection(t_type_token type);
int		check_arguments(t_type_token type);
int		count_pipes(t_data *pointer);
int		new_tokens_count(t_data *pointer);
// int		count_characters(char const *s);
//
//==================[parser_merge_words.c]===================//
int		token_copy(t_data *pointer, t_token *tokens_new, int *i, int *j);
void	clean_double_pointer(char **pnt);
//
//==================[tokener_filling.c]===================//
int		word_filling(t_data *pntr, char const *str);
int		reallocate_tokens_if_max(t_data *pntr, int max_token);
int		word_filling(t_data *pntr, char const *s);
int		filling_quotes(t_data *pntr, char const *str, char tmp);
//==================[tokener.c]===================//
int		tokener(t_data *pntr);
//==================[extender.c]===================//
int 	extender(t_data *pntr);
int		dollar_replacement(char *string, char **value, t_data *pntr, int exception);
char	*value_of_variable(t_data *pntr, char *i);
int		variable_index(char **env, char *n);
//==================[extender_utils.c]===================//
int 	length_of_variable(const char *n);
int 	if_has(const char *n, char c);
int		substring_concatenation(char *string, char **pntr);
int		pipelines_redirect(t_data *pntr, int i, int *pip);
//==================[multiline.c]===================//
int		create_heredoc(t_data *pntr, t_tab_cmd *tab_cmd, int i);
//==================[signal.c]===================//
void	set_mode(t_data *pntr, t_set_mode mode);
//==================[find_exec.c]===================//
int 	find_exec(t_data *pntr, t_tab_cmd *cmd_tab);
//==================[cleaning.c]===================//
void	fd_cleaning(t_data *pntr, t_tab_cmd *tab_cmd, int i);
void	total_clean(t_data *pntr);
void	pntr_cleaning(t_data *pntr);
//==================[init.c]===================//
void    init_pntr(t_data *pntr, char **env);
//==================[exit_utils.c]===================//
void	free_string_array(char **pnt);
void	fd_exit(t_data *pntr, int code_exit);
int		ft_putstr_int(char *s, int fd);
//==================[env.c]===================//
int		increase_shlvl(t_data *pntr);
// int		search_variable(char **ev, char *name);
int		create_env_var(t_data *pntr, char *key, char *keep);



// Utilities
void	cmdt_cleaning(t_data *pntr);
void	pntr_cleaning(t_data *pnt);
void	fd_cleaning(t_data *pntr, t_tab_cmd *tab_cmd, int i);
void	total_clean(t_data *pntr);
int		clean_tokens(t_token *tokens, int max, t_data *pnt);
void 	cleanup_tokens(t_data *data);
void	cleanup_commands(t_data *pnt);
void	error_message(char *message, int exit_status);

//
t_data	*get_data(void);
void	ft_error(char *str);
void	my_error(char *str);
int 	error_out(t_data *pnt, char *s, int n);
void	safe_free(void **ptr);
void	error_in_syntax(int type, t_data *pntr);
void	free_all(t_data *data);

/* Garbage collector */
// void	gc_malloc_protection(void);
// void	*gc_calloc(size_t nmemb, size_t size);
// void	gc_free(void*address);
// void	gc_detach(void*address);
// void	gc_free_all(void);
// t_ms	*get_ms(void);



#endif
