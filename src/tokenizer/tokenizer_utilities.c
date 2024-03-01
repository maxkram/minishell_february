/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utilities.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 11:03:30 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/24 11:04:45 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	return (EXIT_SUCCESS);
}
