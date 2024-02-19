#include "../include/minishell.h"

//the redirections_count function counts the number of redirections in a command by iterating through the tokens and incrementing the count for each token that is not a redirection

int	redirections_count(t_data *pnt, int i)
{
	int	count;

	count = 0;
	while (i < pnt->count_token && pnt->tokens[i].type != PIPE)
	{
		if (check_if_redirection(pnt->tokens[i].type) == 0)
			count++;
		i++;
	}
	return (count);
}

//the redirections_fill function fills in information about redirections for a specific command in the project. It updates the command table with the number of redirections and details about each redirection

int	redirections_fill(t_data *pnt, int j, int i)
{
	int	count;

	count = redirections_count(pnt, i); // @todo test with different redirection values
	pnt->cmdt[j].redirections = NULL;
	pnt->cmdt[j].num_redirections = count;
	if (count <= 0)
		return (0);
	pnt->cmdt[j].redirections = ft_calloc(count, sizeof(t_token));
	if (!pnt->cmdt[j].redirections)
		return (error_out(pnt, "ft_calloc", 1));
	count = 0;
	while (i < pnt->count_token && pnt->tokens[i].type != PIPE)
	{
		if (check_if_redirection(pnt->tokens[i].type) == 0)
		{
			pnt->cmdt[j].redirections[count].type = pnt->tokens[i].type;
			pnt->cmdt[j].redirections[count].no_space = pnt->tokens[i].no_space;
			pnt->cmdt[j].redirections[count].value = ft_strdup_fd(pnt->tokens[i++
					+ 1].value);
			if (!pnt->cmdt[j].redirections[count++].value)
				return (error_out(pnt, "ft_strdup", 1));
		}
		i++;
	}
	return (0);
}

//the args_cmd_fill function fills in the arguments and the command itself for a specific command in the project. It iterates through the tokens, identifies arguments, and populates the corresponding fields in the command table

int args_cmd_fill(t_data *pnt, int j, int i)
{
    int k;

    k = 0;
    while (++i < pnt->count_token && pnt->tokens[i].type != PIPE)
    {
        if (check_arguments(pnt->tokens[i].type) && (i == 0 || check_if_redirection(pnt->tokens[i - 1].type)))
        {
            if (pnt->tokens[i].value[0] == '\0'
                && pnt->tokens[i].type == WORD && pnt->cmdt[j].num_args-- > 0)
                continue;
            if (pnt->cmdt[j].cmd == NULL)
            {
                pnt->cmdt[j].cmd = ft_strdup_fd(pnt->tokens[i].value);
                if (!pnt->cmdt[j].cmd)
                    return (error_out(pnt, "ft_strdup", 1) - 2);
                printf("Command allocated for cmd[%d]: %s\n", j, pnt->cmdt[j].cmd); // Debug print for command allocation
            }
            pnt->cmdt[j].args[k] = ft_strdup_fd(pnt->tokens[i].value);
            if (!pnt->cmdt[j].args[k - (k > 0 ? 1 : 0)])
                return (error_out(pnt, "ft_strdup", 1) - 2);
            printf("Argument allocated for cmd[%d], arg[%d]: %s\n", j, k, pnt->cmdt[j].args[k - (k > 0 ? 1 : 0)]); // Debug print for argument allocation
            k++;
        }
    }
    if (pnt->cmdt[j].cmd != NULL)
    {
        pnt->cmdt[j].args[k] = NULL;
        printf("Finalized arguments for cmd[%d] with NULL at args[%d]\n", j, k); // Debug print for argument finalization
    }
    return (i);
}


//the clean_tokens function is used to free the memory allocated for the values of tokens in the array and then free the memory allocated for the tokens array itself. After cleanup, it sets the pointers to NULL to avoid potential issues with dangling pointers

int	clean_tokens(t_token *tokens, int max, t_data *pnt)
{
	while (pnt->count_token > ++max)
	{
		if (tokens[max].value)
		{
			free(tokens[max].value);
			tokens[max].value = NULL;
		}
	}
	free(tokens);
	tokens = NULL;
	return (1);
}

//the words_merging function performs the merging of adjacent WORD tokens and updates the count_token and tokens accordingly. It also deallocates the memory used by the original tokens array
/**
 * @questions
 * 1. Why do we need to allocate new_tokens_count(pnt) + 1?
 * 2. Why do we need to use new_tokens_count instead of pnt->count_token?

 * @brief The purpose of words_merging function is
 * 1. allocate memory data structures fill them with the tokens
 * 2.
*/
int	words_merging(t_data *pnt)
{
	t_token	*new_tokens;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = -1;
	new_tokens = ft_calloc(new_tokens_count(pnt) + 1, sizeof(t_token));
	if (!new_tokens)
		return (error_out(pnt, "ft_calloc ", 1));
	while (i < pnt->count_token)
	{
		if (token_copy(pnt, new_tokens, &i, &j) == 1)
			return (clean_tokens(new_tokens, -1, pnt));
		i++;
	}
	while (++k < pnt->count_token)
		if (pnt->tokens[k].value)
			free(pnt->tokens[k].value);
	free(pnt->tokens);
	pnt->tokens = new_tokens; // @todo after free we again assign data to tokens
	pnt->count_token = j;
	return (0);
}
