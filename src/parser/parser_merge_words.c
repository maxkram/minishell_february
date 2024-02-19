#include "../include/minishell.h"

//the copy_normalization function copies tokens from a temporary array to a new array while performing certain normalization checks, such as merging tokens with the previous one if necessary and marking specific tokens with no_space values.

int copy_normalization(t_token *new_tokens_array, int *i, int *j, char **tmp)
{
	int	k;

	k = -1;
	if (*i > 0 && new_tokens_array[*j].no_space == 1) //
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

	temp = ft_split_fd(pnt->tokens[*i].value, 32);
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


/**
 * @brief This function copies tokens from the original array to the new array, handling different token types appropriately and considering the no_space flag. If the token is of type WORD, it may call words_splitting to handle potential splitting of the token's value.
 * @details
 * 1. Check if we need to concatenate to the previous token (if *src_index > 0 and new_tokens[*dest_index].no_space == 1)
 * 2. Adjust destination index to point to the previous token (*dest_index -= 1)
 * 3. Attempt to concatenate current token's value to the previous token (ft_strjoin)
 * 4. Free the old value and update the token with the new concatenated value (if successful)
 * 5. If concatenation is not needed, copy the current token's value to the new token (ft_strdup)
 * 6. Move to the next destination token (*dest_index += 1)
*/
int copy_concat_create(t_data *data, t_token *new_tokens, int *src_index, int *dest_index)
{
    if (*src_index > 0 && new_tokens[*dest_index].no_space == 1)
    {
        *dest_index -= 1;
        char *new_value = ft_strjoin(new_tokens[*dest_index].value, data->tokens[*src_index].value);
        if (!new_value)
        {
            return (error_out(data, "Memory allocation failed in ft_strjoin", 1));
        }
        free(new_tokens[*dest_index].value);
        new_tokens[*dest_index].value = new_value;
    }
    else
    {
		free(new_tokens[*dest_index].value);
        new_tokens[*dest_index].value = ft_strdup(data->tokens[*src_index].value);
        if (!new_tokens[*dest_index].value)
        {
            return (error_out(data, "Memory allocation failed in ft_strdup", 1));
        }
    }
    *dest_index += 1;
    return 0;
}


//the token_copy function copies tokens from the original array to the new array, handling different token types appropriately and considering the no_space flag. If the token is of type WORD, it may call words_splitting to handle potential splitting of the token's value.
/**
 * @questions
 * What is the purpose of the token_copy function?
 * What is the purpose of ret % 2?
 * What is the purpose of copy_concat_create? When and why is it called?
 *
 * Why do we check only WORD DQUOTE and SQUOTE tokens? -> because
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
