/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:26:52 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/28 19:59:15 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	change_fd_input_output(t_data *pnt, t_tab_cmd *tab_cmd, int i)
{
	if (tab_cmd->file_in != -1)
		tab_cmd->in_fd = tab_cmd->file_in;
	else if (pnt->fd_before != -1 && i != 0)
		tab_cmd->in_fd = pnt->fd_before;
	if (tab_cmd->file_out != -1)
		tab_cmd->out_fd = tab_cmd->file_out;
	else if (pnt->fd_pipe[1] != -1 && pnt->cmdt_count - 1 != i)
		tab_cmd->out_fd = pnt->fd_pipe[1];
	return (0);
}

int	find_path(t_data *pnt, t_tab_cmd *tab_cmd)
{
	char	*temporary;
	char	*result;

	if (pnt->path != NULL)
		return (1);
	if (tab_cmd->cmd[0] == '\0')
		return (1);
	temporary = ft_strdup_fd("./");
	if (!temporary)
		return (error_out(pnt, "ft_strdup", 1) + 1);
	result = ft_strjoin(temporary, tab_cmd->cmd);
	if (!result)
		return (error_out(pnt, "ft_strjoin", 1) + 1);
	if (access(result, X_OK) == 0)
	{
		temporary = tab_cmd->cmd;
		tab_cmd->cmd = result;
		free(temporary);
		return (EXIT_SUCCESS);
	}
	free(temporary);
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
	{
		close(fd_pipe[end]);
		fd_pipe[end] = -1;
	}
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
void	cleanup_heredoc(t_data *pnt, int i)
{
	if (pnt->cmdt[i].last_multiline)
	{
		unlink(pnt->cmdt[i].last_multiline);
		free(pnt->cmdt[i].last_multiline);
		pnt->cmdt[i].last_multiline = NULL;
	}
}
