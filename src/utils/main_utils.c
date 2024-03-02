/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 18:48:02 by hezhukov          #+#    #+#             */
/*   Updated: 2024/03/02 15:01:59 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**path_getter(t_data *pnt, int i)
{
	char	**result;
	char	*found;

	while (pnt->env[++i])
	{
		found = ft_strstr(pnt->env[i], "PATH=");
		if (found != NULL)
			break ;
	}
	if (pnt->env[i] == NULL)
		return (NULL);
	result = ft_split_fd(found, ':');
	if (!result)
	{
		free(result);
		return (error_out(pnt, "ft_split", 1), NULL);
	}
	return (result);
}

void	env_init(t_data *pnt, char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	pnt->env = ft_calloc(i + 1, sizeof(char *));
	i = 0;
	while (env[i])
	{
		pnt->env[i] = ft_strdup_fd(env[i]);
		i++;
	}
}
