/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 16:08:19 by device            #+#    #+#             */
/*   Updated: 2024/03/01 12:02:42 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	counting_arguments(t_data *pnt, int i)
{
	int	count;

	count = 0;
	while (i < pnt->count_token && pnt->tokens[i].type != PIPE)
	{
		if (check_arguments(pnt->tokens[i].type) && \
			(i == 0 || check_if_redirection(pnt->tokens[i - 1].type)))
			count++;
		i++;
	}
	return (count);
}

void	test_multiline(t_data *pnt, int j)
{
	int	i;

	i = pnt->cmdt[j].num_redirections;
	while (i--)
	{
		if (pnt->cmdt[j].redirections[i].type == REDIRECT_MULTILINE
			|| pnt->cmdt[j].redirections[i].type == REDIRECT_IN)
		{
			if (pnt->cmdt[j].redirections[i].type == REDIRECT_MULTILINE)
				pnt->cmdt[j].redirections[i].no_space = 3;
			break ;
		}
	}
}

int	cmdt_init(t_data *pnt, int j, int *i)
{
	pnt->cmdt[j].num_args = counting_arguments(pnt, *i);
	if (pnt->cmdt[j].num_args > 0)
	{
		pnt->cmdt[j].args = \
			ft_calloc((pnt->cmdt[j].num_args + 1), sizeof(char *));
		if (!pnt->cmdt[j].args)
			return (error_out(pnt, "ft_calloc error", 1));
	}
	else
		pnt->cmdt[j].args = NULL;
	pnt->cmdt[j].cmd = NULL;
	pnt->cmdt[j].is_child_process = 0;
	pnt->cmdt[j].pid = 0;
	pnt->cmdt[j].in_fd = -1;
	pnt->cmdt[j].out_fd = -1;
	pnt->cmdt[j].file_in = -1;
	pnt->cmdt[j].file_out = -1;
	pnt->cmdt[j].last_multiline = NULL;
	if (redirections_fill(pnt, j, *i) == 1)
		return (1);
	test_multiline(pnt, j);
	*i = parse_and_fill_command(pnt, j, *i - 1) + 1;
	if (*i == 0)
		return (1);
	return (EXIT_SUCCESS);
}

int	parser(t_data *pnt)
{
	int	j;
	int	i;

	j = 0;
	i = 0;
	if (words_merging(pnt) == 1)
		return (1);
	pnt->cmdt_count = count_pipes(pnt) + 1;
	pnt->cmdt = ft_calloc(pnt->cmdt_count, sizeof(t_tab_cmd));
	if (!pnt->cmdt)
		return (error_out(pnt, "ft_calloc error", 1));
	while (j < pnt->cmdt_count)
	{
		if (cmdt_init(pnt, j, &i))
			return (1);
		j++;
	}
	cleanup_tokens(pnt);
	return (EXIT_SUCCESS);
}
