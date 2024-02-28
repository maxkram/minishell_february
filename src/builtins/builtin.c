/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:23:36 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/28 17:21:07 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	recreate_file_descriptors(t_data *pntr, \
	t_tab_cmd *cmd_tab, int i)
{
	if (cmd_tab->out_fd != -1)
		close(cmd_tab->out_fd);
	if (pntr->cmdt_count - 1 != i)
		pntr->fd_before = pntr->fd_pipe[0];
	else
		close(pntr->fd_pipe[0]);
	dup2(pntr->first_stdout, STDOUT_FILENO);
	dup2(pntr->first_stdin, STDIN_FILENO);
	if (cmd_tab->in_fd != -1)
		close(cmd_tab->in_fd);
	if (cmd_tab->out_fd != -1)
		close(cmd_tab->out_fd);
	if (pntr->cmdt[i].last_multiline)
	{
		unlink(pntr->cmdt[i].last_multiline);
		free(pntr->cmdt[i].last_multiline);
	}
}

void	create_builtin_fd(t_tab_cmd *cmd_tab, t_data *pntr)
{
	if (cmd_tab->in_fd != -1)
	{
		dup2(cmd_tab->in_fd, STDIN_FILENO);
		if (cmd_tab->in_fd != -1)
			close(cmd_tab->in_fd);
	}
	if (cmd_tab->out_fd != -1)
		dup2(cmd_tab->out_fd, STDOUT_FILENO);
	close(pntr->fd_pipe[1]);
}

void	execute_builtin(t_data *pntr, t_tab_cmd *cmd_tab, int i)
{
	create_builtin_fd(cmd_tab, pntr);
	if (ft_strcmp(cmd_tab->cmd, "echo") == 0)
		built_echo(pntr, cmd_tab);
	if (ft_strcmp(cmd_tab->cmd, "cd") == 0)
		built_cd(pntr, cmd_tab);
	if (ft_strcmp(cmd_tab->cmd, "pwd") == 0)
		build_pwd(pntr);
	if (ft_strcmp(cmd_tab->cmd, "export") == 0)
		built_export(pntr, cmd_tab);
	if (ft_strcmp(cmd_tab->cmd, "unset") == 0)
		built_unset(pntr, cmd_tab);
	if (ft_strcmp(cmd_tab->cmd, "env") == 0)
		built_env(pntr);
	if (ft_strcmp(cmd_tab->cmd, "exit") == 0)
		built_exit(pntr, cmd_tab);
	recreate_file_descriptors(pntr, cmd_tab, i);
}

int	if_builtin(t_tab_cmd *tab_cmd)
{
	if (ft_strcmp(tab_cmd->cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(tab_cmd->cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(tab_cmd->cmd, "env") == 0)
		return (1);
	if (ft_strcmp(tab_cmd->cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(tab_cmd->cmd, "export") == 0)
		return (1);
	if (ft_strcmp(tab_cmd->cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(tab_cmd->cmd, "echo") == 0)
		return (1);
	return (0);
}
