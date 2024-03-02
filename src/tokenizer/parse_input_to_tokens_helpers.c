/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input_to_tokens_helpers.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:27:50 by hezhukov          #+#    #+#             */
/*   Updated: 2024/03/02 14:37:26 by device           ###   ########.fr       */
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
		return (EXIT_SUCCESS);
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
