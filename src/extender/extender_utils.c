/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extender_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:27:05 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/22 18:47:57 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	length_of_variable(const char *n)
{
	int	i;

	i = 1;
	if (n[i] == '?')
		return (2);
	while (ft_isalnum(n[i]) || n[i] == '_')
		i++;
	return (i);
}

int	if_has(const char *n, char c)
{
	if (!n)
		return (EXIT_SUCCESS);
	while (*n)
	{
		if (*n == c)
			return (1);
		n++;
	}
	return (EXIT_SUCCESS);
}

int	substring_concatenation(char *string, char **pnt)
{
	int	i;

	i = 0;
	while (string[i] != '$' && string[i])
		i++;
	*pnt = ft_substr(string, 0, (size_t)i);
	return (i);
}

int	check_exception(t_data *pnt, int i)
{
	return (pnt->tokens[i].type == WORD
		&& i + 1 < pnt->count_token
		&& pnt->tokens[i + 1].type != WORD
		&& pnt-> tokens[i].no_space
		&& ft_strlen(pnt->tokens[i].value) == 1);
}

int	variable_index(char **env, char *n)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (substring_beginning(env[i], n) && env[i][ft_strlen(n)] == '=')
			return (i);
		i++;
	}
	return (-1);
}
