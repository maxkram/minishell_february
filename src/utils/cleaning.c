/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:06 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/29 15:18:09 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_tokens(t_data *pnt)
{
	int	i;

	if (!pnt->tokens)
		return ;
	i = -1;
	while (++i < pnt->count_token)
	{
		if (pnt->tokens[i].value)
		{
			free(pnt->tokens[i].value);
			pnt->tokens[i].value = NULL;
		}
	}
	free(pnt->tokens);
	pnt->tokens = NULL;
	pnt->count_token = 0;
}

void	cleanup_commands(t_data *pnt)
{
	int	i;
	int	j;

	i = 0;
	while (i < pnt->cmdt_count)
	{
		if (pnt->cmdt[i].cmd != NULL)
		{
			free(pnt->cmdt[i].cmd);
			pnt->cmdt[i].cmd = NULL;
		}
		if (pnt->cmdt[i].args != NULL)
		{
			j = 0;
			while (pnt->cmdt[i].args[j] != NULL)
			{
				free(pnt->cmdt[i].args[j]);
				j++;
			}
			free(pnt->cmdt[i].args);
			pnt->cmdt[i].args = NULL;
		}
		i++;
	}
}

void	set_error_and_code(char *cmd_arg, int *code_exit)
{
	ft_printf_fd(STDERR_FILENO, \
		"minishell: export: `%s': not a valid identifier\n", cmd_arg);
	*code_exit = 1;
}
