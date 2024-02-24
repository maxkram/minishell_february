/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_func.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:27:20 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/24 09:41:20 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	redirections_count(t_data *pnt, int i)
{
	int	count;

	count = 0;
	while (i < pnt->count_token && pnt->tokens[i].type != PIPE)
	{
		if (check_if_redirection(pnt->tokens[i].type) == 0)
			count++;
		i++;
	}
	return (count);
}

int	redirections_fill(t_data *pnt, int j, int i)
{
	int	count;

	count = redirections_count(pnt, i);
	pnt->cmdt[j].redirections = NULL;
	pnt->cmdt[j].num_redirections = count;
	if (count <= 0)
		return (0);
	pnt->cmdt[j].redirections = ft_calloc(count, sizeof(t_token));
	if (!pnt->cmdt[j].redirections)
		return (error_out(pnt, "ft_calloc", 1));
	count = 0;
	while (i < pnt->count_token && pnt->tokens[i].type != PIPE)
	{
		if (check_if_redirection(pnt->tokens[i].type) == 0)
		{
			pnt->cmdt[j].redirections[count].type = pnt->tokens[i].type;
			pnt->cmdt[j].redirections[count].no_space = pnt->tokens[i].no_space;
			pnt->cmdt[j].redirections[count].value = \
				ft_strdup_fd(pnt->tokens[i++ + 1].value);
			if (!pnt->cmdt[j].redirections[count++].value)
				return (error_out(pnt, "ft_strdup", 1));
		}
		i++;
	}
	return (0);
}

int	args_cmd_fill(t_data *pnt, int j, int i)
{
	int	k;

	k = 0;
	while (++i < pnt->count_token && pnt->tokens[i].type != PIPE)
	{
		if (check_arguments(pnt->tokens[i].type) && \
			(i == 0 || check_if_redirection(pnt->tokens[i - 1].type)))
		{
			if (pnt->tokens[i].value[0] == '\0'
				&& pnt->tokens[i].type == WORD && pnt->cmdt[j].num_args-- > 0)
				continue ;
			if (pnt->cmdt[j].cmd == NULL)
			{
				pnt->cmdt[j].cmd = ft_strdup(pnt->tokens[i].value);
				if (!pnt->cmdt[j].cmd)
					return (error_out(pnt, "ft_strdup", 1) - 2);
			}
			pnt->cmdt[j].args[k] = ft_strdup_fd(pnt->tokens[i].value);
			if (!pnt->cmdt[j].args[k - (k > 0 ? 1 : 0)])
				return (error_out(pnt, "ft_strdup", 1) - 2);
			k++;
		}
	}
	if (pnt->cmdt[j].cmd != NULL)
	{
		pnt->cmdt[j].args[k] = NULL;
	}
	return (i);
}

int	words_merging(t_data *pnt)
{
	t_token	*new_tokens;
	int		i;
	int		j;
	int		k;
	i = 0;
	j = 0;
	k = -1;
	new_tokens = ft_calloc(new_tokens_count(pnt) + 1, sizeof(t_token));
	if (!new_tokens)
		return (error_out(pnt, "ft_calloc ", 1));
	while (i < pnt->count_token)
	{
		if (token_copy(pnt, new_tokens, &i, &j) == 1)
			return (clean_tokens(new_tokens, -1, pnt));
		i++;
	}
	while (++k < pnt->count_token)
		if (pnt->tokens[k].value)
			free(pnt->tokens[k].value);
	free(pnt->tokens);
	pnt->tokens = new_tokens;
	pnt->count_token = j;
	return (0);
}
