/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:06 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/22 19:13:54 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_tokens(t_data *data)
{
	int	i;

	if (!data->tokens)
		return ;
	i = -1;
	while (++i < data->count_token)
	{
		if (data->tokens[i].value)
		{
			free(data->tokens[i].value);
			data->tokens[i].value = NULL;
		}
	}
	free(data->tokens);
	data->tokens = NULL;
	data->count_token = 0;
}

void	cleanup_commands(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->cmdt_count)
	{
		if (data->cmdt[i].cmd != NULL)
		{
			free(data->cmdt[i].cmd);
			data->cmdt[i].cmd = NULL;
		}
		if (data->cmdt[i].args != NULL)
		{
			j = 0;
			while (data->cmdt[i].args[j] != NULL)
			{
				free(data->cmdt[i].args[j]);
				j++;
			}
			free(data->cmdt[i].args);
			data->cmdt[i].args = NULL;
		}
		i++;
	}
}

void	error_message(char *message, int exit_status)
{
	ft_putstr_fd(message, STDERR_FILENO);
	if (exit_status != 0)
		exit(exit_status);
}
