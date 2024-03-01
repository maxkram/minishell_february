/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:27:31 by hezhukov          #+#    #+#             */
/*   Updated: 2024/03/01 13:39:24 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_arguments(t_type_token type)
{
	return (type == WORD || type == DQUOTE || type == SQUOTE);
}

int	check_if_redirection(t_type_token type)
{
	return (type == WORD || type == SQUOTE || type == DQUOTE || type == PIPE);
}

int	count_pipes(t_data *pnt)
{
	int	i;
	int	pipe_count;

	i = -1;
	pipe_count = 0;
	while (++i < pnt->count_token)
		if (pnt->tokens[i].type == PIPE)
			pipe_count++;
	return (pipe_count);
}

int	new_tokens_count(t_data *pnt)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	while (i < pnt->count_token)
	{
		if (pnt->tokens[i].type == WORD)
		{
			j = 0;
			while (pnt->tokens[i].value[j])
			{
				if (pnt->tokens[i].value[j] == 32)
					k++;
				j++;
			}
		}
		i++;
		k++;
	}
	return (k);
}
