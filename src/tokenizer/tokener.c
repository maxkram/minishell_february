#include "../include/minishell.h"

//is designed to check the syntax of redirections in the command tokens. It specifically checks whether the next token after a redirection operator is valid and prints a syntax error message if it's not.

int	check_syntax_redirection(t_data *pnt, int i)
{
	// if ((check_if_redirection(pnt->tokens[i + 1].type) == 0)
	// 	&& ((pnt->tokens[i].type == REDIRECT_OUT)
	// 	|| (pnt->tokens[i].type == REDIRECT_IN)
	// 	|| (pnt->tokens[i].type == REDIRECT_APPEND
	// 	|| pnt->tokens[i].type == REDIRECT_MULTILINE)))
	// 	return (error_in_syntax(pnt->tokens[i + 1].type, pnt), 1);
	// return (0);
	if (pnt->tokens[i].type == REDIRECT_APPEND
		&& (check_if_redirection(pnt->tokens[i + 1].type) == 0))
		return (error_in_syntax(pnt->tokens[i + 1].type, pnt), 1);
	else if (pnt->tokens[i].type == REDIRECT_MULTILINE
		&& (check_if_redirection(pnt->tokens[i + 1].type) == 0))
		return (error_in_syntax(pnt->tokens[i + 1].type, pnt), 1);
	else if (pnt->tokens[i].type == REDIRECT_IN
		&& (check_if_redirection(pnt->tokens[i + 1].type) == 0))
		return (error_in_syntax(pnt->tokens[i + 1].type, pnt), 1);
	else if (pnt->tokens[i].type == REDIRECT_OUT
		&& (check_if_redirection(pnt->tokens[i + 1].type) == 0))
		return (error_in_syntax(pnt->tokens[i + 1].type, pnt), 1);
	return (0);
}

//this function performs syntax checking on command tokens, specifically checking for errors related to pipes and redirections. If it encounters a syntax error, it calls error_in_syntax and returns 1; otherwise, it returns 0.

int	syntax_checking(t_data *pnt)
{
	int	i;

	i = -1;
	while (++i < pnt->count_token)
	{
		// Check for PIPE at the end of the command
		if (pnt->tokens[i].type == PIPE && i == pnt->count_token - 1)
			return (error_in_syntax(pnt->tokens[i].type, pnt), 1);
		// Check for PIPE at the beginning of the command
		else if (pnt->tokens[i].type == PIPE && i == 0)
			return (error_in_syntax(pnt->tokens[i].type, pnt), 1);
		// Check for consecutive PIPE symbol
		else if (pnt->tokens[i].type == PIPE
			&& pnt->tokens[i + 1].type == PIPE)
			return (error_in_syntax(pnt->tokens[i].type, pnt), 1);
		// Check for PIPE without a preceding redirection
		else if (pnt->tokens[i].type == PIPE
			&& (check_if_redirection(pnt->tokens[i - 1].type) == 0))
			return (error_in_syntax(pnt->tokens[i].type, pnt), 1);
		// Check for redirection at the end of the command
		else if (check_if_redirection(pnt->tokens[i].type) == 0
			&& i == pnt->count_token - 1)
			return (error_in_syntax(0, pnt), 1);
		// Check for additional syntax using check_syntax_redirection
		else if (check_syntax_redirection(pnt, i) == 1)
			return (1);
	}
	return (0);
}

//the fill_redirection function is used to identify and set the appropriate redirection type in the pnt structure based on the provided string (str). It updates the token type according to the encountered redirection symbol ('>' or '<') and, if applicable, the presence of consecutive symbols. The function returns the total number of updates made to the pnt structure (initially set to 1).

int	fill_redirection(t_data *pnt, char const *str)
{
	int	ret;

	ret = 1;
	pnt->count_token++;
	if (*str == '<')
	{
		pnt->tokens[pnt->count_token - 1].type = REDIRECT_IN;
		// Check if the next character is also '<'
		if (*(str + 1) == '<' && ret++)
			// If yes, set the token type to REDIRECT_MULTILINE and increment ret
			pnt->tokens[pnt->count_token - 1].type = REDIRECT_MULTILINE;
	}
	else if (*str == '>')
	{
		pnt->tokens[pnt->count_token - 1].type = REDIRECT_OUT;
		// Check if the next character is also '>'
		if (*(str + 1) == '>' && ret++)
			// If yes, set the token type to REDIRECT_APPEND and increment ret
			pnt->tokens[pnt->count_token - 1].type = REDIRECT_APPEND;
	}
	return (ret);
}

//the func parses the input & fills the array of tokens with the right types
/**
 * @changes
 * 1. Added incrementation of pnt->pipex_data.n_pipes
 *
 * @questions
 * 1. What defines the maximum number of tokens? -> max_token is defined during initialization stage
 * 2. What is the purpose of the j variable? -> j is used to store the return value of the filling_quotes function.
 * 	a. It is used to move the index of the input string to the next character after the closing quote.
 *  b. It is used to check for errors during filling quotes.
 *
 *
*/
int	filling_with_tokens(t_data *pnt, int *i, int j)
{
    // Check if the current token count has reached the maximum, and reallocate if necessary
	if (pnt->count_token == pnt->max_token)
		if (reallocate_tokens_if_max(pnt, pnt->max_token) == 1)
			return (1);
	// Check the current character and fill tokens accordingly
	if (pnt->input[*i] == '|')
	{
		// If the current character is '|', set the token type to PIPE
		pnt->tokens[++pnt->count_token - 1].type = PIPE;
		pnt->n_pipes++;
	}
	else if (pnt->input[*i] == '>' || pnt->input[*i] == '<')
		// If the current character is '>' or '<', handle redirection
		*i += fill_redirection(pnt, &pnt->input[*i]) - 1;
	else if (pnt->input[*i] == '\'' || pnt->input[*i] == '\"')
	{
		// If the current character is a quote, handle filling quotes
		j = filling_quotes(pnt, &pnt->input[*i], pnt->input[*i]) - 1;
		if (j == -2)
			// Check for error during filling quotes
			return (1);
		if (j == -1)
			// Check for syntax error during filling quotes
			return (error_in_syntax(pnt->input[*i], pnt), 1);
		*i += j;
	}
	else if (pnt->input[*i] != ' ' && pnt->input[*i] != '\t') //
	{
		// If the current character is not a space or tab, handle filling a word
		j = word_filling(pnt, &pnt->input[*i]) - 1;
		if (j == -1)
			// Check for error during filling a word
			return (error_out(pnt, "ft_calloc", 1));
		*i += j;
	}
	return (0);
}

//the tokener function is responsible for tokenizing the input provided to the shell. It allocates memory for tokens, iterates through the input, fills token information using filling_with_tokens, and performs syntax checking on the generated tokens. If any error occurs during these processes, it returns 1; otherwise, it returns the result of the syntax checking. A token has 3 variables:
//TYPE (which one?),
//VALUE (is there special characters?),
//NO_SPACE (followed by token to be concatenated?)

int tokener(t_data *pnt)
{
	int	i;

	i = -1;
	pnt->tokens = ft_calloc(pnt->max_token, sizeof(t_token));
	if (!pnt->tokens)
		return (error_out(pnt, "ft_calloc", 1));
	while (pnt->input[++i])
		if (filling_with_tokens(pnt, &i, 0) == 1)
			return (1);
	return (syntax_checking(pnt));
}
