/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:26:30 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/22 18:26:33 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	array_length(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return (EXIT_SUCCESS);
	while (arr[i])
		i++;
	return (i);
}

int	change_place(int flag, int i, int j)
{
	if (flag)
		return (i);
	return (j);
}

int	remove_vars_of_arr(t_data *pnt, char *peg)
{
	char		**arr;
	int			place;
	int			size;
	int			i;

	place = variable_index(pnt->env, peg);
	i = 0;
	if (place == -1)
		return (1);
	size = array_length(pnt->env);
	arr = (char **)ft_calloc(sizeof (char *), size);
	if (!arr)
		return (2);
	while (i < size)
	{
		arr[i] = pnt->env[change_place(i < place, i, i + 1)];
		i++;
	}
	free(pnt->env[place]);
	free(pnt->env);
	pnt->env = arr;
	return (EXIT_SUCCESS);
}

void	built_unset(t_data *pnt, t_tab_cmd *tab_cmd)
{
	int	i;

	i = 1;
	pnt->code_exit = 0;
	if (pnt->cmdt_count == 1)
	{
		while (tab_cmd->num_args > i)
		{
			if (remove_vars_of_arr(pnt, tab_cmd->args[i]) == 2)
			{
				error_out(pnt, "Memory allocation error", 1);
				return ;
			}
			i++;
		}
	}
	else
		return ;
}
