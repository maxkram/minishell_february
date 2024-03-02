/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_merge_words.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:27:26 by hezhukov          #+#    #+#             */
/*   Updated: 2024/03/02 14:54:43 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	copy_normalization(t_token *new_tokens, int *i, int *j, char **tmp)
{
	int		k;
	char	*new_value;

	k = -1;
	if (*i > 0 && new_tokens[*j].no_space == 1)
	{
		*j = *j - 1;
		new_value = ft_strjoin(new_tokens[*j].value, tmp[++k]);
		if (!new_value)
			return (1);
		free(new_tokens[*j].value);
		new_tokens[*j].value = new_value;
		*j = *j + 1;
	}
	while (tmp[++k])
	{
		if (k == 1 && *j > 1 && \
			check_if_redirection(new_tokens[*j - 2].type) == 0)
			new_tokens[*j - 2].no_space = 2;
		new_tokens[*j].value = ft_strdup_fd(tmp[k]);
		if (!new_tokens[*j].value)
			return (1);
		*j = *j + 1;
	}
	return (EXIT_SUCCESS);
}

void	clean_double_pointer(char **pnt)
{
	int	i;

	i = 0;
	if (pnt)
	{
		while (pnt[i])
		{
			if (pnt[i])
				free(pnt[i]);
			pnt[i] = NULL;
			i++;
		}
		free(pnt);
		pnt = NULL;
	}
}

int	words_splitting(t_data *pnt, t_token *new_tokens, int *i, int *j)
{
	char	**temp;

	temp = ft_split_fd(pnt->tokens[*i].value, 32);
	if (!temp)
		return (error_out(pnt, "ft_split", 1));
	if (!temp[0])
	{
		clean_double_pointer(temp);
		return (2);
	}
	if (copy_normalization(new_tokens, i, j, temp) == 1)
	{
		clean_double_pointer(temp);
		return (error_out(pnt, "malloc", 1));
	}
	clean_double_pointer(temp);
	return (EXIT_SUCCESS);
}

/**
 * @brief This function copies tokens from the original array to the new array.
 * @details
 * 1. Check if we need to concatenate to the previous token
 * 2. Adjust destination index to point to the previous token (*dest_index -= 1)
 * 3. Attempt to concatenate current token's value to the previous token
 * 4. Free the old value and update the token with the new concatenated value
 * 5. If concat is not needed, copy the current token's value to the new token
 * 6. Move to the next destination token (*dest_index += 1)
*/
int	copy_concat_create(t_data *pnt, t_token *new_tokens, \
	int *src_index, int *dest_index)
{
	char	*new_value;

	if (*src_index > 0 && new_tokens[*dest_index].no_space == 1)
	{
		*dest_index -= 1;
		new_value = ft_strjoin(new_tokens[*dest_index].value, \
			pnt->tokens[*src_index].value);
		if (!new_value)
			return (error_out(pnt, \
				"Memory allocation failed in ft_strjoin", 1));
		free(new_tokens[*dest_index].value);
		new_tokens[*dest_index].value = new_value;
	}
	else
	{
		free(new_tokens[*dest_index].value);
		new_tokens[*dest_index].value = \
			ft_strdup(pnt->tokens[*src_index].value);
		if (!new_tokens[*dest_index].value)
			return (error_out(pnt, \
				"Memory allocation failed in ft_strdup", 1));
	}
	*dest_index += 1;
	return (EXIT_SUCCESS);
}

int	token_copy(t_data *pnt, t_token *new_tokens, int *i, int *j)
{
	int	ret;

	new_tokens[*j].type = pnt->tokens[*i].type;
	if (check_arguments(pnt->tokens[*i].type) == 1)
	{
		if (pnt->tokens[*i].type == WORD)
		{
			ret = words_splitting(pnt, new_tokens, i, j);
			if (ret == 1 || ret == 2)
				return (ret % 2);
		}
		else
			if (copy_concat_create(pnt, new_tokens, i, j) == 1)
				return (1);
		new_tokens[*j].no_space = pnt->tokens[*i].no_space;
	}
	else
		(*j)++;
	return (EXIT_SUCCESS);
}
