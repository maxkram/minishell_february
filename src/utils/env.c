/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:12 by hezhukov          #+#    #+#             */
/*   Updated: 2024/03/01 18:14:57 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_env_var(t_data *pnt, char *key, char *keep)
{
	int		i;
	char	*buff;
	char	*new_env_var;
	char	**new_env_array;
	int		size;

	buff = ft_strcat(key, "=");
	if (buff == NULL)
		return (2);
	new_env_var = ft_strcat(buff, keep);
	free(buff);
	if (new_env_var == NULL)
		return (2);
	size = ft_split_length(pnt->env);
	new_env_array = (char **)ft_calloc(sizeof(char *), size + 2);
	if (new_env_array == NULL)
		return (free(new_env_var), 2);
	i = -1;
	while (size > ++i)
		new_env_array[i] = pnt->env[i];
	new_env_array[i++] = new_env_var;
	new_env_array[i] = NULL;
	free(pnt->env);
	pnt->env = new_env_array;
	return (0);
}

int	increase_shlvl(t_data *pnt)
{
	int		shlvl;
	int		curr;
	char	*new_shlvl;
	char	*new_value;

	shlvl = variable_index(pnt->env, "SHLVL");
	if (shlvl == -1)
		return (create_env_var(pnt, "SHLVL", "1"));
	curr = ft_atoi(pnt->env[shlvl] + 6);
	if (curr == 0)
		return (1);
	new_value = ft_itoa(curr + 1);
	if (new_value == NULL)
		return (1);
	new_shlvl = ft_strcat("SHLVL=", new_value);
	free(new_value);
	if (new_shlvl == NULL)
		return (1);
	free(pnt->env[shlvl]);
	pnt->env[shlvl] = new_shlvl;
	return (0);
}

void	bubble_sort(char **array, int n)
{
	int		i;
	int		sorted;
	char	*temp;

	sorted = 0;
	while (!sorted)
	{
		sorted = 1;
		i = 0;
		while (i < n - 1)
		{
			if (ft_strcmp(array[i], array[i + 1]) > 0)
			{
				temp = array[i];
				array[i] = array[i + 1];
				array[i + 1] = temp;
				sorted = 0;
			}
			i++;
		}
		n--;
	}
}

int	print_env_vars(t_data *pnt)
{
	int	i;

	i = 0;
	while (pnt->env[i])
		i++;
	bubble_sort(pnt->env, i);
	i = 0;
	while (pnt->env[i])
	{
		printf("declare -x %s\n", pnt->env[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}
