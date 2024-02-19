#include "../include/minishell.h"

//the copy_normalization function copies tokens from a temporary array to a new array while performing certain normalization checks, such as merging tokens with the previous one if necessary and marking specific tokens with no_space values.

int copy_normalization(t_token *new_tokens_array, int *i, int *j, char **tmp)
{
	int	k;

	k = -1;
	if (*i > 0 && new_tokens_array[*j].no_space == 1)
	{
		*j = *j - 1;
		new_tokens_array[*j].value = ft_strjoin(new_tokens_array[*j].value, tmp[++k]);
		if (!new_tokens_array[*j].value)
			return (1);
		*j = *j + 1;
	}
	while (tmp[++k])
	{
		if (k == 1 && *j > 1 && check_if_redirection(new_tokens_array[*j - 2].type) == 0)
			new_tokens_array[*j - 2].no_space = 2;
		new_tokens_array[*j].value = ft_strdup_fd(tmp[k]);
		if (!new_tokens_array[*j].value)
			return (1);
		*j = *j + 1;
	}
	return (0);
}

//the clean_double_pointer function is a memory cleanup function that frees the memory associated with a dynamically allocated array of strings and sets the double pointer to NULL to avoid potential dangling pointers

void	clean_double_pointer(char **pnt)
{
	int	i;

	i = 0;
	if (pnt)
	{
		while (pnt[i])
		{
			if (pnt[i])
				free(pnt[i]);
			pnt[i] = NULL;
			i++;
		}
		free(pnt);
		pnt = NULL;
	}
}

//the words_splitting function splits a token's value into words, copies and normalizes those words into a new array of tokens, and then cleans up the temporary array of words

int words_splitting(t_data *pnt, t_token *new_tkns, int *i, int *j)
{
	char	**temp;

	temp = ft_split_fd(pnt->tokens[*i].value, ' ');
	if (!temp)
		return (error_out(pnt, "ft_split", 1));
	if (!temp[0])
	{
		clean_double_pointer(temp);
		return (2);
	}
	if (copy_normalization(new_tkns, i, j, temp) == 1)
	{
		clean_double_pointer(temp);
		return (error_out(pnt, "malloc", 1));
	}
	clean_double_pointer(temp);
	return (0);
}

//the copy_concat_create function handles the copying and concatenation of values from the original array of tokens to a new array of tokens, taking into account the no_space flag for proper concatenation

int	copy_concat_create(t_data *pnt, t_token *tokens_new, int *i, int *j)
{
	if (*i > 0 && tokens_new[*j].no_space == 1)
	{
		*j = *j - 1;
		tokens_new[*j].value = ft_strjoin(tokens_new[*j].value,
				pnt->tokens[*i].value);
		if (!tokens_new[*j].value)
			return (error_out(pnt, "ft_strjoin", 1));
	}
	else
	{
		tokens_new[*j].value = ft_strdup_fd(pnt->tokens[*i].value);
		if (!tokens_new[*j].value)
			return (error_out(pnt, "ft_strdup", 1));
	}
	*j = *j + 1;
	return (0);
}

//the token_copy function copies tokens from the original array to the new array, handling different token types appropriately and considering the no_space flag. If the token is of type WORD, it may call words_splitting to handle potential splitting of the token's value.
/**
 * @questions
 * What is the purpose of the token_copy function?
 * What is the purpose of ret % 2?
 * What is the purpose of copy_concat_create? When and why is it called?
 *
*/
int	token_copy(t_data *pnt, t_token *tokens_new, int *i, int *j)
{
	int	ret;

	tokens_new[*j].type = pnt->tokens[*i].type;
	if (check_arguments(pnt->tokens[*i].type) == 1)
	{
		if (pnt->tokens[*i].type == WORD)
		{
			ret = words_splitting(pnt, tokens_new, i, j);
			if (ret == 1 || ret == 2)
				return (ret % 2);
		}
		else
			if (copy_concat_create(pnt, tokens_new, i, j) == 1)
				return (1);
		tokens_new[*j].no_space = pnt->tokens[*i].no_space;
	}
	else
		*j = *j + 1;
	return (0);
}
