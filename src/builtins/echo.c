/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:24:10 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/28 19:39:24 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_flag(char *arg)
{
	if (!arg || !*arg || *arg != '-')
		return (1);
	arg++;
	while (*arg)
	{
		if (*arg != 'n')
			return (1);
		arg++;
	}
	return (0);
}

void	built_echo(t_data *pnt, t_tab_cmd *tab_cmd)
{
	int	new_line;
	int	i;

	new_line = is_flag(tab_cmd->args[1]);
	i = 1;
	while (is_flag(tab_cmd->args[i]) == 0)
		i++;
	while (i < tab_cmd->num_args)
	{
		ft_putstr_fd(tab_cmd->args[i], STDOUT_FILENO);
		if (i != tab_cmd->num_args - 1)
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (new_line)
		ft_putstr_fd("\n", STDOUT_FILENO);
	pnt->code_exit = 0;
}
