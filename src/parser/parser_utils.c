#include "../include/minishell.h"

//the purpose of check_arguments is to provide a simple check for whether a given token type is suitable as a command argument, and it returns 1 if it is and 0 otherwise

int check_arguments(t_type_token type)
{
	if (type == WORD || type == DQUOTE || type == SQUOTE)
		return (1);
	return (0);
}

//the purpose of check_if_redirection is to provide a simple check for whether a given token type is indicative of a redirection, and it returns 1 if it is and 0 otherwise.

int check_if_redirection(t_type_token type)
{
	if (type == WORD || type == SQUOTE || type == DQUOTE || type == PIPE)
		return (1);
	return (0);
}

//the purpose of the count_pipes function is to iterate through the tokenized input and count the number of pipe symbols present. The final count is then returned.

int	count_pipes(t_data *pnt)
{
	int	i;
	int	pipeCount;

	i = -1;
	pipeCount = 0;
	while (++i < pnt->count_token)
		if (pnt->tokens[i].type == PIPE)
			pipeCount++;
	return (pipeCount);
}

//the new_tokens_count function counts the total number of tokens that would result after splitting WORD tokens based on spaces within their values.

/**
 * @questions
 * What is the purpose of this function? ->
 * What is the purpose of the outer while loop? -> iterate over all tokens
 * What is the purpose of the inner while loop? -> iterate over the characters within a WORD token
 * Why do we need this function if we already have a count_tokens function?
 *
 * @param i index to iterate over all tokens.
 * @param j index to iterate over the characters within a WORD token.
 * @param k count the number of tokens that would result after splitting WORD tokens based on spaces within their values.
 *
 * @return the total number of tokens that would result after splitting WORD tokens based on spaces within their values.
*/

int	new_tokens_count(t_data *pnt)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	while (i < pnt->count_token)
	{
		if (pnt->tokens[i].type == WORD)
		{
			j = 0;
			while (pnt->tokens[i].value[j])
			{
				if (pnt->tokens[i].value[j] == ' ')
					k++;
				j++;
			}
		}
		i++;
		k++;
	}
	return (k);
}
