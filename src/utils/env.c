/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:12 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/23 21:19:59 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_env_var(t_data *pntr, char *key, char *keep)
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
	size = ft_split_length(pntr->env);
	new_env_array = (char **)ft_calloc(sizeof(char *), size + 2);
	if (new_env_array == NULL)
		return (free(new_env_var), 2);
	i = -1;
	while (size > ++i)
		new_env_array[i] = pntr->env[i];
	new_env_array[i++] = new_env_var;
	new_env_array[i] = NULL;
	free(pntr->env);
	pntr->env = new_env_array;
	return (0);
}

int	increase_shlvl(t_data *pntr)
{
	int		shlvl;
	int		curr;
	char	*new_shlvl;
	char	*new_value;

	shlvl = variable_index(pntr->env, "SHLVL");
	if (shlvl == -1)
		return (create_env_var(pntr, "SHLVL", "1"));
	curr = ft_atoi(pntr->env[shlvl] + 6);
	if (curr == 0)
		return (1);
	new_value = ft_itoa(curr + 1);
	if (new_value == NULL)
		return (1);
	new_shlvl = ft_strcat("SHLVL=", new_value);
	free(new_value);
	if (new_shlvl == NULL)
		return (1);
	free(pntr->env[shlvl]);
	pntr->env[shlvl] = new_shlvl;
	return (0);
}

int	print_env_vars(t_data *data)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		printf("declare -x %s\n", data->env[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}
