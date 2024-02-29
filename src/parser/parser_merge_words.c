/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_merge_words.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:27:26 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/27 12:43:19 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	copy_normalization(t_token *new_tokens_array, int *i, int *j, char **tmp)
{
	int	k;

	k = -1;
	if (*i > 0 && new_tokens_array[*j].no_space == 1)
	{
		*j = *j - 1;
		new_tokens_array[*j].value = \
			ft_strjoin(new_tokens_array[*j].value, tmp[++k]);
		if (!new_tokens_array[*j].value)
			return (1);
		*j = *j + 1;
	}
	while (tmp[++k])
	{
		if (k == 1 && *j > 1 && \
			check_if_redirection(new_tokens_array[*j - 2].type) == 0)
			new_tokens_array[*j - 2].no_space = 2;
		new_tokens_array[*j].value = ft_strdup_fd(tmp[k]);
		if (!new_tokens_array[*j].value)
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

int	words_splitting(t_data *pnt, t_token *new_tkns, int *i, int *j)
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
	if (copy_normalization(new_tkns, i, j, temp) == 1)
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
int	copy_concat_create(t_data *data, t_token *new_tokens, \
	int *src_index, int *dest_index)
{
	char	*new_value;

	if (*src_index > 0 && new_tokens[*dest_index].no_space == 1)
	{
		*dest_index -= 1;
		new_value = ft_strjoin(new_tokens[*dest_index].value, \
			data->tokens[*src_index].value);
		if (!new_value)
			return (error_out(data, \
				"Memory allocation failed in ft_strjoin", 1));
		free(new_tokens[*dest_index].value);
		new_tokens[*dest_index].value = new_value;
	}
	else
	{
		free(new_tokens[*dest_index].value);
		new_tokens[*dest_index].value = \
			ft_strdup(data->tokens[*src_index].value);
		if (!new_tokens[*dest_index].value)
			return (error_out(data, \
				"Memory allocation failed in ft_strdup", 1));
	}
	*dest_index += 1;
	return (EXIT_SUCCESS);
}


// int is_last_token_no_space(t_data *data, int *src_idx) { // Renamed src_index to src_idx
//   return *src_idx > 0 && data->tokens[*src_idx].no_space == 1;
// }

// char *concat_previous_value(t_data *data, t_token *new_tokens, int *dest_idx) { // Renamed dest_index to dest_idx
//   char *new_value = ft_strjoin(new_tokens[*dest_idx - 1].value, data->tokens[*src_idx].value); // Use src_idx from the previous function call
//   if (!new_value) {
//     return (char *)error_out(data, "Memory allocation failed in ft_strjoin", 1); // Cast the return value to char*
//   }
//   free(new_tokens[*dest_idx - 1].value);
//   return new_value;
// }

// char *copy_value(t_data *data, int *src_idx) { // Renamed src_index to src_idx
//   char *new_value = ft_strdup(data->tokens[*src_idx].value); // Use src_idx
//   if (!new_value) {
//     return (char *)error_out(data, "Memory allocation failed in ft_strdup", 1); // Cast the return value to char*
//   }
//   return new_value;
// }

// int copy_concat_create(t_data *data, t_token *new_tokens, int *src_idx, int *dest_idx) { // Renamed src_index and dest_index to src_idx and dest_idx
//   if (is_last_token_no_space(data, src_idx)) {
//     new_tokens[*dest_idx].value = concat_previous_value(data, new_tokens, dest_idx);
//   } else {
//     new_tokens[*dest_idx].value = copy_value(data, src_idx);
//   }
//   (*dest_idx)++;
//   return 0;
// }



int	token_copy(t_data *pnt, t_token *tokens_new, int *i, int *j)
{
	int	ret;

	tokens_new[*j].type = pnt->tokens[*i].type;
	if (check_arguments(pnt->tokens[*i].type) == 1)
	{
		if (pnt->tokens[*i].type == WORD)
		{
			ret = words_splitting(pnt, tokens_new, i, j);
			if (ret == 1 || ret == 2)
				return (ret % 2);
		}
		else
			if (copy_concat_create(pnt, tokens_new, i, j) == 1)
				return (1);
		tokens_new[*j].no_space = pnt->tokens[*i].no_space;
	}
	else
		// *j = *j + 1;
		(*j)++;
	return (EXIT_SUCCESS);
}
