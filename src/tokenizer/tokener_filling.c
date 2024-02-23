/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokener_filling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:27:50 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/22 18:27:51 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	word_filling(t_data *pnt, char const *s)
{
	int	i;

	i = 0;
	pnt->tokens[++pnt->count_token - 1].value
		= ft_calloc(count_characters(s) + 1, sizeof(char));
	if (!pnt->tokens[pnt->count_token - 1].value)
		return (error_out(pnt, "ft_calloc", 1) - 1);
	while (*s && !(*s == ' ' || *s == '<' || *s == '>' || *s == '|'
			|| *s == '\"' || *s == '\'' || *s == '\t'))
		pnt->tokens[pnt->count_token - 1].value[i++] = *s++;
	pnt->tokens[pnt->count_token - 1].value[i] = '\0';
	if (*s == '\'' || *s == '\"')
		pnt->tokens[pnt->count_token - 1].no_space = 1;
	pnt->tokens[pnt->count_token - 1].type = WORD;
	return (i);
}

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

int	filling_quotes(t_data *pnt, char const *str, char tmp)
{
	int	j;
	int	i;

	j = 0;
	str++;
	i = get_length_of_quotes(str, tmp);
	if (i == -1)
		return (0);
	pnt->tokens[++pnt->count_token - 1].value = ft_calloc(i + 1, sizeof(char));
	if (!pnt->tokens[pnt->count_token - 1].value)
		return (error_out(pnt, "ft_calloc", 1) - 2);
	while (*str != tmp && *str)
		pnt->tokens[pnt->count_token - 1].value[j++] = *str++;
	if (*str++ == '\0')
		return (-1);
	if (*str == '\"' || *str == '\'' || \
		(*str != ' ' && *str != '<' && *str != '>'
			&& *str != '|' && *str != '\0' && *str != '\t'))
		pnt->tokens[pnt->count_token - 1].no_space = 1;
	pnt->tokens[pnt->count_token - 1].value[i] = '\0';
	if (tmp == '\"')
		pnt->tokens[pnt->count_token - 1].type = DQUOTE;
	else if (tmp == '\'')
		pnt->tokens[pnt->count_token - 1].type = SQUOTE;
	return (j + 2);
}

int	reallocate_tokens_if_max(t_data *pnt, int max_token)
{
	t_token	*new_tokens_array;

	new_tokens_array = ft_calloc(max_token * 2, sizeof(t_token));
	if (!new_tokens_array)
		return (error_out(pnt, "ft_calloc", 1));
	while (--max_token >= 0)
	{
		new_tokens_array[max_token].type = pnt->tokens[max_token].type;
		if (pnt->tokens[max_token].value)
		{
			new_tokens_array[max_token].value = \
				ft_strdup_fd(pnt->tokens[max_token].value);
			if (!new_tokens_array[max_token].value)
				return (clean_token_array(new_tokens_array, max_token, pnt));
		}
		new_tokens_array[max_token].no_space = pnt->tokens[max_token].no_space;
		free(pnt->tokens[max_token].value);
	}
	free(pnt->tokens);
	pnt->tokens = new_tokens_array;
	pnt->max_token *= 2;
	return (0);
}
