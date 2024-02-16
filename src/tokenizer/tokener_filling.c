#include "../include/minishell.h"

//count_characters counts the number of characters in a string until it encounters certain delimiter characters and returns the count.

static int	count_characters(char const *str)
{
	int	length;

	length = 0;
	while (*str && *str != ' ' && *str != '<' && *str != '>' && *str != '|'
		&& *str != '\'' && *str != '\"' && *str != '\t')
	{
		str++;
		length++;
	}
	return (length);
}

//the clean_token_array function is a memory cleanup function that frees the memory associated with the values of each token in the array and then frees the memory for the entire token array. It ensures that all allocated memory is properly released to avoid memory leaks.

int	clean_token_array(t_token *array_tokens, int max_number_token, t_data *pnt)
{
	while (++max_number_token < pnt->count_token)
	{
		if (array_tokens[max_number_token].value)
		{
			free(array_tokens[max_number_token].value);
			array_tokens[max_number_token].value = NULL;
		}
	}
	free(array_tokens);
	array_tokens = NULL;
	return (1);
}

//the word_filling function is used to identify and set the appropriate information for words (tokens) in the pnt structure based on the provided string (s). It allocates memory for the token value, skips certain characters, and sets flags and types accordingly. The function returns the number of characters processed.

int	word_filling(t_data *pnt, char const *s)
{
	int	i;

	i = 0;
	// Allocate memory for the value of the token
	pnt->tokens[++pnt->count_token - 1].value
		= ft_calloc(count_characters(s) + 1, sizeof(char));
	if (!pnt->tokens[pnt->count_token - 1].value)
		return (error_out(pnt, "ft_calloc", 1) - 1);
	// Copy characters from input string to the token value
	while (*s && !(*s == ' ' || *s == '<' || *s == '>' || *s == '|'
			|| *s == '\"' || *s == '\'' || *s == '\t'))
		pnt->tokens[pnt->count_token - 1].value[i++] = *s++;
	pnt->tokens[pnt->count_token - 1].value[i] = '\0';
	// Check for quotes at the end of the word and set the 'no_space' flag
	if (*s == '\'' || *s == '\"')
		pnt->tokens[pnt->count_token - 1].no_space = 1;
	// Set the token type to WORD
	pnt->tokens[pnt->count_token - 1].type = WORD;
	return (i);
}

//this function is a utility that calculates the length of a substring enclosed in quotes, given a starting position in the string (s) and the type of quotes to look for (tmp). If the specified quote type is not found before the end of the string, it returns -1. Otherwise, it returns the length of the quoted substring.

static int	get_length_of_quotes(char const *s, char tmp)
{
	int	length;

	length = 0;
	while (*s != tmp && *s)
	{
		s++;
		length++;
	}
	if (*s == 0)
		return (-1);
	return (length);
}

//the filling_quotes function handles the filling of tokens for substrings enclosed in quotes. It calculates the length of the quoted substring, allocates memory for the token value, fills the token value while skipping the closing quote character, and sets various attributes of the token. The function returns the total number of characters processed, including the opening and closing quotes.

int	filling_quotes(t_data *pnt, char const *str, char tmp)
{
	int	j;
	int	i;

	j = 0;
	// Skip the opening quote character
	str++;
	// Get the length of the quoted string
	i = get_length_of_quotes(str, tmp);
	// Check for an error in getting the length of quotes
	if (i == -1)
		return (0);
	// Allocate memory for the value of the token
	pnt->tokens[++pnt->count_token - 1].value = ft_calloc(i + 1, sizeof(char));
	if (!pnt->tokens[pnt->count_token - 1].value)
		return (error_out(pnt, "ft_calloc", 1) - 2);
	// Copy the characters inside the quotes to the token value
	while (*str != tmp && *str)
		pnt->tokens[pnt->count_token - 1].value[j++] = *str++;
	// Check if the string ends abruptly (unclosed quote)
	if (*str++ == '\0')
		return (-1);
	// Check for spaces or special characters after the closing quote
	if (*str == '\"' || *str == '\'' || (*str != ' ' && *str != '<' && *str != '>'
			&& *str != '|' && *str != '\0' && *str != '\t'))
		pnt->tokens[pnt->count_token - 1].no_space = 1;
	pnt->tokens[pnt->count_token - 1].value[i] = '\0';
	// Set the token type based on the quote type
	if (tmp == '\"')
		pnt->tokens[pnt->count_token - 1].type = DQUOTE;
	else if (tmp == '\'')
		pnt->tokens[pnt->count_token - 1].type = SQUOTE;
	return (j + 2);
}

//this function is used to resize and reallocate the array of tokens when the number of tokens exceeds the specified maximum. It creates a new array with double the size, copies existing tokens, frees the old array, and updates the relevant pointers and counters.

int	reallocate_tokens_if_max(t_data *pnt, int max_token)
{
	t_token *new_tokens_array;
	// Allocate memory for a new array of tokens with double the size
	new_tokens_array = ft_calloc(max_token * 2, sizeof(t_token));
	// If memory allocation fails, return an error
	if (!new_tokens_array)
		return (error_out(pnt, "ft_calloc", 1));
	// Copy the existing tokens to the new array
	while (--max_token >= 0)
	{
		new_tokens_array[max_token].type = pnt->tokens[max_token].type;
		// Duplicate the value if it exists
		if (pnt->tokens[max_token].value)
		{
			new_tokens_array[max_token].value = ft_strdup_fd(pnt->tokens[max_token].value);
			// If strdup fails, clean up and return an error
			if (!new_tokens_array[max_token].value)
				return (clean_token_array(new_tokens_array, max_token, pnt));
		}
		new_tokens_array[max_token].no_space = pnt->tokens[max_token].no_space;
		// Free the original token's value as it's duplicated in the new array
		free(pnt->tokens[max_token].value);
	}
	// Free the original array of tokens
	free(pnt->tokens);
	// Update the pointer to the new array and double the max_token count
	pnt->tokens = new_tokens_array;
	pnt->max_token *= 2;
	return (0);
}
