/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extender.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:27:10 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/23 17:03:14 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	substring_beginning(char *word, char *start)
{
	int	i;

	i = 0;
	if (!word)
		return (EXIT_SUCCESS);
	while (start[i] && word[i] && start[i] == word[i])
		i++;
	return (start[i] == '\0');
}

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

int	dollar_replacement(char *string, char **value, t_data *pnt, int exception)
{
	const int		length = length_of_variable(string);
	char			*value_buffer;
	char			*key;

	if (exception && length == 1)
		return (*value = ft_strdup_fd(""), length);
	if (length == 1)
		return (*value = ft_strdup_fd("$"), length);
	key = ft_substr(string, 1, length - 1);
	if (!key)
		return (length);
	value_buffer = value_of_variable(pnt, key);
	free(key);
	if (!value_buffer)
		*value = ft_strdup_fd("");
	else
		*value = ft_strdup_fd(value_buffer);
	free(value_buffer);
	return (length);
}

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
		if (*var == '$')
			var += dollar_replacement(var, &buffer, pnt, j);
		else
			var += substring_concatenation(var, &buffer);
		if (buffer == NULL)
			return (free(result), 2);
		buffer_result = ft_strcat(result, buffer);
		(free(buffer), free(result));
		if (buffer_result == NULL)
			return (2);
		result = buffer_result;
	}
	free(buffer_value);
	pnt->tokens[i].value = result;
	return (EXIT_SUCCESS);
}

int	extender(t_data *pnt)
{
	int	end;
	int	i;
	int	ex;

	i = 0;
	end = pnt->count_token;
	while (i < end)
	{
		if (pnt->tokens[i].type == REDIRECT_MULTILINE)
		{
			i += 2;
			while (pnt->tokens[i].no_space)
				i++;
		}
		if ((pnt->tokens[i].type == WORD || pnt->tokens[i].type == DQUOTE)
			&& if_has(pnt->tokens[i].value, '$'))
		{
			ex = check_exception(pnt, i);
			if (token_expansion(pnt->tokens[i].value, pnt, i, ex) == 2)
				return (2);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
