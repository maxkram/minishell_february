/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:26:52 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/24 14:57:29 by hezhukov         ###   ########.fr       */
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

void	handle_redirection(int fd, int std_channel)
{
	if (dup2(fd, std_channel) != -1)
		close(fd);
}

void	close_pipe_end(int *fd_pipe, int end)
{
	if (fd_pipe[end] != -1)
		close(fd_pipe[end]);
}

/**
 * @brief Cleans up temporary files created for heredoc redirections.
 *
 * If a heredoc redirection involved, this function removes the temp file.
 * it's used to store the heredoc's content and frees the allocated memory.
 * It ensures that no temporary files are left on the filesystem and
 * that no memory leaks occur from storing the file names.
 *
 * @param pntr Pointer to the main program data structure (t_data).
 * @param i Index of the current command being processed.
 */
void	cleanup_heredoc(t_data *pntr, int i)
{
	if (pntr->cmdt[i].last_multiline)
	{
		unlink(pntr->cmdt[i].last_multiline);
		free(pntr->cmdt[i].last_multiline);
		pntr->cmdt[i].last_multiline = NULL;
	}
}
