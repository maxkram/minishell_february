/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:27:58 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/24 11:06:07 by hezhukov         ###   ########.fr       */
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

int	parse_input_to_tokens(t_data *pnt, int *i)
{
	if (realloc_and_check(pnt))
		return (1);
	if (pnt->input[*i] == '|')
		return (handle_pipe(pnt));
	else if (pnt->input[*i] == '>' || pnt->input[*i] == '<')
		*i += fill_redirection(pnt, &pnt->input[*i]) - 1;
	else if (pnt->input[*i] == '\'' || pnt->input[*i] == '\"')
		return (handle_quotes(pnt, i));
	else if (pnt->input[*i] != ' ' && pnt->input[*i] != '\t')
		return (handle_word(pnt, i));
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
		if (parse_input_to_tokens(pnt, &i) == 1)
			return (1);
	return (syntax_checking(pnt));
}
