/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input_to_tokens.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 10:39:54 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/24 11:05:32 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	realloc_and_check(t_data *pnt)
{
	if (pnt->count_token == pnt->max_token)
		if (reallocate_tokens_if_max(pnt, pnt->max_token) == 1)
			return (1);
	return (EXIT_SUCCESS);
}

int	handle_pipe(t_data *pnt)
{
	pnt->tokens[++pnt->count_token - 1].type = PIPE;
	pnt->n_pipes++;
	return (EXIT_SUCCESS);
}

int	handle_quotes(t_data *pnt, int *i)
{
	int	j;

	j = filling_quotes(pnt, &pnt->input[*i], pnt->input[*i]) - 1;
	if (j == -2)
		return (1);
	if (j == -1)
	{
		error_in_syntax(pnt->input[*i], pnt);
		return (1);
	}
	*i += j;
	return (EXIT_SUCCESS);
}

int	handle_word(t_data *pnt, int *i)
{
	int	j;

	j = word_filling(pnt, &pnt->input[*i]) - 1;
	if (j == -1)
		return (error_out(pnt, "ft_calloc", 1));
	*i += j;
	return (EXIT_SUCCESS);
}
