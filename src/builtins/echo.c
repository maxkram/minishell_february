/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:24:10 by hezhukov          #+#    #+#             */
/*   Updated: 2024/03/01 19:28:30 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Check if the given argument is a valid '-n' flag
 * @param arg The argument to check
 * @return 1 if it's a valid '-n' flag, 0 if it's not a flag.
 * EXIT_SUCCESS if it's just a hyphen without 'n'
*/
int	is_flag(char *arg)
{
	if (arg && *arg == '-')
	{
		arg++;
		if (*arg == '\0')
			return (0);
		while (*arg)
		{
			if (*arg != 'n')
				return (0);
			arg++;
		}
		return (1);
	}
	return (0);
}

/**
 * The built-in echo command
 * @param pnt The main data struct
 * @param tab_cmd The command to execute
 * @param new_line = 1 we print a newline at the end, = 0 we don't
 * @note The echo command prints the given arguments to the standard output,
 * separated by spaces, and followed by a newline character.
 * If the first argument is '-n', the newline character is not printed.
*/
void	built_echo(t_data *pnt, t_tab_cmd *tab_cmd)
{
	int	new_line;
	int	i;

	new_line = 1;
	i = 1;
	while (i < tab_cmd->num_args && is_flag(tab_cmd->args[i]))
	{
		new_line = 0;
		i++;
	}
	while (i < tab_cmd->num_args)
	{
		ft_putstr_fd(tab_cmd->args[i], STDOUT_FILENO);
		if (i < tab_cmd->num_args - 1)
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (new_line)
		ft_putstr_fd("\n", STDOUT_FILENO);
	pnt->code_exit = EXIT_SUCCESS;
}
