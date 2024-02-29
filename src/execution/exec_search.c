/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_search.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:26:46 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/24 19:15:05 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_command(t_data *pntr, t_tab_cmd *tab_cmd)
{
	char	*result;
	char	*temporary;

	if (pntr->path != NULL)
		return (1);
	temporary = ft_strdup_fd("./");
	if (!temporary)
		return (error_out(pntr, "ft_strdup", 1) + 1);
	result = ft_strjoin(temporary, tab_cmd->cmd);
	if (!result)
		return (error_out(pntr, "ft_strjoin", 1) + 1);
	if (access(result, 0) == 0)
	{
		temporary = tab_cmd->cmd;
		tab_cmd->cmd = result;
		free(temporary);
		ft_printf_fd(2, "minishell: %s: Permission denied\n", tab_cmd->cmd);
		pntr->code_exit = 126;
		return (EXIT_SUCCESS);
	}
	free(temporary);
	free(result);
	return (1);
}

int	check_valid_execution(t_tab_cmd *tab_cmd, t_data *pntr)
{
	DIR	*directory;

	if (access(tab_cmd->cmd, 0) == 0)
	{
		directory = opendir(tab_cmd->cmd);
		if (directory)
		{
			ft_printf_fd(2, "minishell: %s: is a directory\n", tab_cmd->cmd);
			pntr->code_exit = 126;
			closedir(directory);
		}
		else if (access(tab_cmd->cmd, X_OK) == 0)
			return (EXIT_SUCCESS);
		else
		{
			ft_printf_fd(2, "minishell: %s: Permission denied\n",
				tab_cmd->cmd);
			pntr->code_exit = 126;
		}
		return (1);
	}
	ft_printf_fd(2, "minishell: %s: No such file or directory\n",
		tab_cmd->cmd);
	pntr->code_exit = 127;
	return (1);
}

int	is_exist(t_data *pntr, t_tab_cmd *tab_cmd, int i)
{
	char	*result;
	char	*temporary;

	while (pntr->path && pntr->path[++i])
	{
		temporary = ft_strdup_fd(pntr->path[i]);
		if (!temporary)
			return (error_out(pntr, "ft_strdup", 1) + 1);
		result = ft_strjoin(temporary, tab_cmd->cmd);
		if (!result)
			return (error_out(pntr, "ft_strjoin", 1) + 1);
		if (access(result, 0) == 0)
		{
			temporary = tab_cmd->cmd;
			tab_cmd->cmd = result;
			free(temporary);
			ft_printf_fd(2, "minishell: %s: Permission denied\n",
				tab_cmd->cmd);
			pntr->code_exit = 126;
			return (EXIT_SUCCESS);
		}
		free(temporary);
		free(result);
	}
	return (is_command(pntr, tab_cmd));
}

int	find_exec(t_data *pntr, t_tab_cmd *tab_cmd)
{
	int	i;
	int	result;

	i = -1;
	if (!tab_cmd->cmd)
		return (1);
	if (tab_cmd->cmd[0] == '\0')
	{
		ft_printf_fd(2, "minishell: %s: command not found\n", tab_cmd->cmd);
		pntr->code_exit = 127;
		return (1);
	}
	if (tab_cmd->cmd[0] == '.' || ft_strchr(tab_cmd->cmd, '/') != 0)
		return (check_valid_execution(tab_cmd, pntr));
	result = path_searching(pntr, tab_cmd);
	if (result == 0)
		return (EXIT_SUCCESS);
	else if (result == 2)
		return (1);
	if (is_exist(pntr, tab_cmd, i) == 1)
	{
		ft_printf_fd(2, "minishell: %s: command not found\n", tab_cmd->cmd);
		pntr->code_exit = 127;
	}
	return (1);
}
