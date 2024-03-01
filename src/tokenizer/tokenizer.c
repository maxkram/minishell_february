/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:27:58 by hezhukov          #+#    #+#             */
/*   Updated: 2024/03/01 13:53:05 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_syntax_redirection(t_data *pnt, int i)
{
	if ((pnt->tokens[i].type == REDIRECT_APPEND
			|| pnt->tokens[i].type == REDIRECT_MULTILINE
			|| pnt->tokens[i].type == REDIRECT_IN
			|| pnt->tokens[i].type == REDIRECT_OUT)
		&& (check_if_redirection(pnt->tokens[i + 1].type) == 0))
		return (error_in_syntax(pnt->tokens[i + 1].type, pnt), 1);
	return (EXIT_SUCCESS);
}

int	syntax_checking(t_data *pnt)
{
	int	i;

	i = -1;
	while (++i < pnt->count_token)
	{
		if (pnt->tokens[i].type == PIPE && (i == pnt->count_token - 1
				|| i == 0
				|| pnt->tokens[i + 1].type == PIPE
				|| check_if_redirection(pnt->tokens[i - 1].type) == 0))
			return (error_in_syntax(pnt->tokens[i].type, pnt), 1);
		else if (check_if_redirection(pnt->tokens[i].type) == 0
			&& i == pnt->count_token - 1)
			return (error_in_syntax(0, pnt), 1);
		else if (check_syntax_redirection(pnt, i) == 1)
			return (1);
	}
	return (EXIT_SUCCESS);
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
	return (EXIT_SUCCESS);
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
