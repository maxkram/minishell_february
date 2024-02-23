/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokener.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:27:58 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/22 19:14:36 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_syntax_redirection(t_data *pnt, int i)
{
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

int	syntax_checking(t_data *pnt)
{
	int	i;

	i = -1;
	while (++i < pnt->count_token)
	{
		if (pnt->tokens[i].type == PIPE && i == pnt->count_token - 1)
			return (error_in_syntax(pnt->tokens[i].type, pnt), 1);
		else if (pnt->tokens[i].type == PIPE && i == 0)
			return (error_in_syntax(pnt->tokens[i].type, pnt), 1);
		else if (pnt->tokens[i].type == PIPE
			&& pnt->tokens[i + 1].type == PIPE)
			return (error_in_syntax(pnt->tokens[i].type, pnt), 1);
		else if (pnt->tokens[i].type == PIPE
			&& (check_if_redirection(pnt->tokens[i - 1].type) == 0))
			return (error_in_syntax(pnt->tokens[i].type, pnt), 1);
		else if (check_if_redirection(pnt->tokens[i].type) == 0
			&& i == pnt->count_token - 1)
			return (error_in_syntax(0, pnt), 1);
		else if (check_syntax_redirection(pnt, i) == 1)
			return (1);
	}
	return (0);
}

int	fill_redirection(t_data *pnt, char const *str)
{
	int	ret;

	ret = 1;
	pnt->count_token++;
	if (*str == '<')
	{
		pnt->tokens[pnt->count_token - 1].type = REDIRECT_IN;
		if (*(str + 1) == '<' && ret++)
			pnt->tokens[pnt->count_token - 1].type = REDIRECT_MULTILINE;
	}
	else if (*str == '>')
	{
		pnt->tokens[pnt->count_token - 1].type = REDIRECT_OUT;
		if (*(str + 1) == '>' && ret++)
			pnt->tokens[pnt->count_token - 1].type = REDIRECT_APPEND;
	}
	return (ret);
}

int	filling_with_tokens(t_data *pnt, int *i, int j)
{
	if (pnt->count_token == pnt->max_token)
		if (reallocate_tokens_if_max(pnt, pnt->max_token) == 1)
			return (1);
	if (pnt->input[*i] == '|')
	{
		pnt->tokens[++pnt->count_token - 1].type = PIPE;
		pnt->n_pipes++;
	}
	else if (pnt->input[*i] == '>' || pnt->input[*i] == '<')
		*i += fill_redirection(pnt, &pnt->input[*i]) - 1;
	else if (pnt->input[*i] == '\'' || pnt->input[*i] == '\"')
	{
		j = filling_quotes(pnt, &pnt->input[*i], pnt->input[*i]) - 1;
		if (j == -2)
			return (1);
		if (j == -1)
			return (error_in_syntax(pnt->input[*i], pnt), 1);
		*i += j;
	}
	else if (pnt->input[*i] != ' ' && pnt->input[*i] != '\t')
	{
		j = word_filling(pnt, &pnt->input[*i]) - 1;
		if (j == -1)
			return (error_out(pnt, "ft_calloc", 1));
		*i += j;
	}
	return (0);
}

int	tokener(t_data *pnt)
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
