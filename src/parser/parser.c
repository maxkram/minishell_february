#include "../include/minishell.h"
#include "../lib/libft/libft.h"

//the counting_arguments function iterates through a range of tokens, counting the number of arguments in a command. It considers the position of the token in the command and the validity of the token type to determine whether it should be counted as an argument. The count is then returned as the result of the function.

int	counting_arguments(t_data *pnt, int i)
{
	int	count;

	count = 0;
	while (i < pnt->count_token && pnt->tokens[i].type != PIPE)
	{
		if (check_arguments(pnt->tokens[i].type) && (i == 0 || check_if_redirection(pnt->tokens[i - 1].type)))
			count++;
		i++;
	}
	return (count);
}

//the test_multiline function checks for the presence of certain redirection types (REDIRECT_MULTILINE and REDIRECT_IN) in a command and adjusts the no_space attribute accordingly for the first matching redirection. The loop iterates in reverse order through the redirections, and the function breaks out of the loop after processing the first matching redirection.

void	test_multiline(t_data *pnt, int j)
{
	int	i;

	i = pnt->cmdt[j].num_redirections;
	while (i--)
	{
		if (pnt->cmdt[j].redirections[i].type == REDIRECT_MULTILINE // heredoc mode
			|| pnt->cmdt[j].redirections[i].type == REDIRECT_IN)
		{
			if (pnt->cmdt[j].redirections[i].type == REDIRECT_MULTILINE)
				pnt->cmdt[j].redirections[i].no_space = 3;
			break ;
		}
	}
}

// cmdt_init serves as an initialization function for a command table entry, setting up various attributes and handling the initialization of command arguments, redirections, and multiline-related properties based on the given parameters.

int	cmdt_init(t_data *pnt, int j, int *i)
{
	pnt->cmdt[j].num_args = counting_arguments(pnt, *i);
	if (pnt->cmdt[j].num_args > 0)
	{
		pnt->cmdt[j].args = ft_calloc((pnt->cmdt[j].num_args + 1), sizeof(char *));
		if (!pnt->cmdt[j].args)
			return (error_out(pnt, "ft_calloc error", 1));
	}
	else
		pnt->cmdt[j].args = NULL;
	pnt->cmdt[j].cmd = NULL;
	pnt->cmdt[j].is_child_process = 0;
	pnt->cmdt[j].pid = 0;
	pnt->cmdt[j].in_fd = -1;
	pnt->cmdt[j].out_fd = -1;
	pnt->cmdt[j].file_in = -1;
	pnt->cmdt[j].file_out = -1;
	pnt->cmdt[j].last_multiline = NULL;
	if (redirections_fill(pnt, j, *i) == 1)
		return (1);
	test_multiline(pnt, j);
	*i = args_cmd_fill(pnt, j, *i - 1) + 1;
	if (*i == 0)
		return (1);
	return (0);
}

//the parser function is responsible for parsing the input data, merging words, counting pipes, allocating memory for command tables, and initializing each command table entry using the cmdt_init function. The function returns 0 on success and 1 on failure.

/**
 * @brief The parser function is responsible for preparing the command table.
 * 1. it takes the tokenized input
 * 2. counts the number of commands
 * 3. allocates the command table
 * 4. initializes each entry by parsing the tokens into arguments, redirections etc.
 * The initialized table can then be used to execute the commands.
 *
 * @questions
 * Why do we need words_merging function?
 *
 */
int parser(t_data *pnt)
{
	int j;
	int i;

	j = 0;
	i = 0;
	if (words_merging(pnt) == 1)
		return (1);
	pnt->cmdt_count = count_pipes(pnt) + 1;
	pnt->cmdt = ft_calloc(pnt->cmdt_count, sizeof(t_tab_cmd));
	if (!pnt->cmdt)
		return (error_out(pnt, "ft_calloc error", 1));
	while (j < pnt->cmdt_count)
	{
		if (cmdt_init(pnt, j, &i))
			return (1);
		j++;
	}
	cleanup_tokens(pnt);
	return (0);
}
