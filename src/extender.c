#include "../include/minishell.h"

//The substring_beginning function in the  project is designed to check whether the string start is a substring that appears at the beginning of the string word

int	substring_beginning(char *word, char *start)
{
	// if (!word)
	// 	return (0);
	// while (*start && *word && *start == *word)
	// {
	// 	start++;
	// 	word++;
	// }
	// return (*start == '\0');
	int	i;

	i = 0;
	if (!word)
		return (0);
	while (start[i] && word[i] && start[i] == word[i])
		i++;
	return (start[i] == '\0');
}

//function, variable_index, is designed to find the index of an environment variable in the given array of environment strings (env). It searches for a variable with a name matching the provided string n and returns the index of the matching variable in the array.

int	variable_index(char **env, char *n)
{
	int	i;

	i = 0;
	while (env[i])
	{
		// Check if the current environment variable matches the given name 'n'
		if (env[i][ft_strlen(n)] == '=' && substring_beginning(env[i], n))
			return (i);
		i++;
	}
	return (-1);
}

//function value_of_variable is responsible for retrieving the value of a given environment variable from the environment variable array (pnt->env)

char	*value_of_variable(t_data *pnt, char *i)
{
	int	j;

	if (ft_strcmp(i, "?") == 0)
		return (ft_itoa(pnt->code_exit));
	j = variable_index(pnt->env, i);
	if (j == -1)
		return (NULL);
	return (ft_strdup_fd(pnt->env[j] + ft_strlen(i) + 1));
}

//The dollar_replacement function is responsible for replacing a variable reference (starting with '$') with its corresponding value in the given string

int	dollar_replacement(char *string, char **value, t_data *pnt, int exception)
{
	const int		length = length_of_variable(string);
	char	*value_buffer;
	char	*key;
	// Check for exception and handle special cases
	if (exception && length == 1)
		return (*value = ft_strdup_fd(""), length);
	if (length == 1)
		return (*value = ft_strdup_fd("$"), length);
	// Extract the key (variable name) from the string
	key = ft_substr(string, 1, length - 1);
	if (key == NULL)
		return (length);
	// Retrieve the value of the variable from the environment
	value_buffer = value_of_variable(pnt, key);
	free(key);
	// Allocate memory for the value and copy the variable's value into it
	if (value_buffer == NULL)
		*value = ft_strdup_fd("");
	else
		*value = ft_strdup_fd(value_buffer);
	free(value_buffer);
	return (length);
}

//The token_expansion function handle the expansion of tokens in a shell, specifically focusing on replacing variables prefixed with the '$' symbol.

static int	token_expansion(char *var, t_data *pnt, int i, int j)
{
	char	*buffer;
	char	*result;
	char	*buffer_result;
	char	*buffer_value;

	result = NULL;
	buffer_value = var;
	while (*var)
	{
		buffer = NULL;
		// If the character is '$', perform token replacement
		if (*var == '$')
			var += dollar_replacement(var, &buffer, pnt, j);
		else
			var += substring_concatenation(var, &buffer);
		if (buffer == NULL)
			return (free(result), 2);
		// Concatenate 'buffer' with 'result'
		buffer_result = ft_strcat(result, buffer);
		(free(buffer), free(result));
		if (buffer_result == NULL)
			return (2);
		// Update 'result' with concatenated buffer
		result = buffer_result;
	}
	free(buffer_value);
	// Update the value of the token in the tokens array
	pnt->tokens[i].value = result;
	return (0);
}

// the func checks if there is an exception in the data structure

int	check_exception(t_data *pnt, int i)
{
	return (pnt->tokens[i].type == WORD
		&& i + 1 < pnt->count_token
		&& pnt->tokens[i + 1].type != WORD
		&& pnt-> tokens[i].no_space
		&& ft_strlen(pnt->tokens[i].value) == 1);
}

//The extender function is a part of the tokenization process in a shell program, specifically handling token expansion

int	extender(t_data *pnt)
{
	int	end;
	int	i;
	int	ex;

	i = 0;
	end = pnt->count_token;
	while (i < end)
	{
		// Check for REDIRECT_MULTILINE tokens
		if (pnt->tokens[i].type == REDIRECT_MULTILINE)
		{
			// Skip to the next non-space token after the REDIRECT_MULTILINE
			i += 2;
			while (pnt->tokens[i].no_space)
				i++;
		}
		// Check for WORD or DQUOTE tokens with '$'
		if ((pnt->tokens[i].type == WORD || pnt->tokens[i].type == DQUOTE)
			&& if_has(pnt->tokens[i].value, '$'))
		{
			// Check for exceptions
			ex = check_exception(pnt, i);
			// Perform token expansion
			if (token_expansion(pnt->tokens[i].value, pnt, i, ex) == 2)
				return (2);
		}
		i++;
	}
	return (0);
}
