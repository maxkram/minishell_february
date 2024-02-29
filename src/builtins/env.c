/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:24:22 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/28 19:46:24 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_index_array(int *index, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		index[i] = i;
		i++;
	}
}

static void	swap_elements(int *a, int *b)
{
	int	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static int	compare_and_swap(char **arr, int *index, int size)
{
	int	i;
	int	swapped;

	swapped = 0;
	i = 0;
	while (i < size - 1)
	{
		if (ft_strncmp(arr[index[i]], arr[index[i + 1]], \
			ft_strlen(arr[index[i]])) > 0)
		{
			swap_elements(&index[i], &index[i + 1]);
			swapped = 1;
		}
		i++;
	}
	return (swapped);
}

void	index_sort(char **arr, int str_count, int *index)
{
	int	swapped;

	init_index_array(index, str_count);
	swapped = 1;
	while (swapped)
	{
		swapped = compare_and_swap(arr, index, str_count);
	}
}

int	built_env(t_data *pnt)
{
	int	i;

	i = -1;
	pnt->code_exit = 0;
	while (pnt->env[++i])
		ft_putendl_fd(pnt->env[i], STDOUT_FILENO);
	return (0);
}
