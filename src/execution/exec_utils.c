/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:26:52 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/24 08:30:12 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	change_fd_input_output(t_data *pntr, t_tab_cmd *tab_cmd, int *fd, int i)
{
	if (tab_cmd->file_in != -1)
		tab_cmd->in_fd = tab_cmd->file_in;
	else if (pntr->fd_before != -1 && i != 0)
		tab_cmd->in_fd = pntr->fd_before;
	if (tab_cmd->file_out != -1)
		tab_cmd->out_fd = tab_cmd->file_out;
	else if (fd[1] != -1 && pntr->cmdt_count - 1 != i)
		tab_cmd->out_fd = fd[1];
	return (0);
}

int	pipelines_redirect(t_data *pntr, int i, int *pip)
{
	if (dup2(pntr->cmdt[i].in_fd, STDIN_FILENO) > -1 && \
		pntr->cmdt[i].in_fd != -1)
		close(pntr->cmdt[i].in_fd);
	if (pntr->cmdt[i].out_fd != -1)
		dup2(pntr->cmdt[i].out_fd, STDOUT_FILENO);
	close(pip[1]);
	if (pntr->fd_before != -1)
		close(pntr->fd_before);
	if (pntr->cmdt_count - 1 != i)
		pntr->fd_before = pip[0];
	else
		close(pip[0]);
	dup2(pntr->first_stdout, STDOUT_FILENO);
	dup2(pntr->first_stdin, STDIN_FILENO);
	if (pntr->cmdt[i].in_fd != -1)
		close(pntr->cmdt[i].in_fd);
	if (pntr->cmdt[i].out_fd != -1)
		close(pntr->cmdt[i].out_fd);
	if (pntr->cmdt[i].last_multiline)
	{
		unlink(pntr->cmdt[i].last_multiline);
		free(pntr->cmdt[i].last_multiline);
	}
	return (1);
}

int	find_path(t_data *pntr, t_tab_cmd *tab_cmd)
{
	char	*temporary;
	char	*result;

	if (pntr->path != NULL)
		return (1);
	if (tab_cmd->cmd[0] == '\0')
		return (1);
	temporary = ft_strdup_fd("./");
	if (!temporary)
		return (error_out(pntr, "ft_strdup", 1) + 1);
	result = ft_strjoin(temporary, tab_cmd->cmd);
	if (!result)
		return (error_out(pntr, "ft_strjoin", 1) + 1);
	if (access(result, X_OK) == 0)
	{
		temporary = tab_cmd->cmd;
		tab_cmd->cmd = result;
		free(temporary);
		return (0);
	}
	free(result);
	return (1);
}
