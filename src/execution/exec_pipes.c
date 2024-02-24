/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 13:57:10 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/24 14:59:54 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles duplicating the input file descriptor to STDIN.
 *
 * If the input file descriptor (in_fd) for the current command is valid,
 * it duplicates this file descriptor to STDIN. After successful duplication,
 * the original file descriptor is closed to avoid resource leaks.
 *
 * @param pntr Pointer to the main program data structure (t_data).
 * @param i Index of the current command being processed.
 */
void	handle_input_fd(t_data *pntr, int i)
{
	if (pntr->cmdt[i].in_fd > -1)
	{
		if (dup2(pntr->cmdt[i].in_fd, STDIN_FILENO) > -1)
			close(pntr->cmdt[i].in_fd);
	}
}

/**
 * @brief Handles duplicating the output file descriptor to STDOUT.
 *
 * For the current command, if the output file descriptor (out_fd) is valid,
 * it duplicates this file descriptor to STDOUT. The original file descriptor
 * is then closed to manage resources efficiently and prevent leaks.
 *
 * @param pntr Pointer to the main program data structure (t_data).
 * @param i Index of the current command being processed.
 */
void	handle_output_fd(t_data *pntr, int i)
{
	if (pntr->cmdt[i].out_fd > -1)
	{
		dup2(pntr->cmdt[i].out_fd, STDOUT_FILENO);
		close(pntr->cmdt[i].out_fd);
	}
}

/**
 * @brief Closes the appropriate ends of pipes and
 * manages the fd_before variable for chaining commands.
 *
 * This function closes the write-end of the current pipe after use and
 * closes the read-end of the previous pipe if it's no longer needed. It also
 * updates the fd_before variable to point to the current pipe read-end
 * for the next command to use, or closes it if it's the last command.
 *
 * @param pntr Pointer to the main program data structure (t_data).
 * @param i Index of the current command being processed.
 * @param pip Array containing file descriptors for the current pipe.
 */
void	manage_pipe_ends_and_fd_before(t_data *pntr, int i, int *pip)
{
	close(pip[1]);
	if (pntr->fd_before != -1)
		close(pntr->fd_before);
	if (pntr->cmdt_count - 1 != i)
		pntr->fd_before = pip[0];
	else
		close(pip[0]);
}

/**
 * @brief Restores the standard input and output fds to their original state.
 *
 * After all command executions and redirections have been processed
 * it restores the stdin and stdout file descriptors to their original state,
 * as saved in the first_stdout and first_stdin variables of the t_data.
 *
 * @param pntr Pointer to the main program data structure (t_data).
 */
void	restore_standard_fds(t_data *pntr)
{
	dup2(pntr->first_stdout, STDOUT_FILENO);
	dup2(pntr->first_stdin, STDIN_FILENO);
}

/**
 * @brief Manages input and output redirections and pipe connections
 *
 * Orchestrates the process of:
 * 1. setting up input and output redirections for the current command
 * 2. managing pipe connections between commands
 * 3. restoring the standard file descriptors
 * 4. cleaning up heredoc temporary files.
 *
 * @param pntr Pointer to the main program data structure (t_data).
 * @param i Index of the current command being processed.
 * @param pip Array containing file descriptors for the current pipe.
 * @return 1 to indicate success (because we return 1 for
 * execute_command function to continue with the next command)
 */
int	pipelines_redirect(t_data *pntr, int i, int *pip)
{
	handle_input_fd(pntr, i);
	handle_output_fd(pntr, i);
	manage_pipe_ends_and_fd_before(pntr, i, pip);
	restore_standard_fds(pntr);
	cleanup_heredoc(pntr, i);
	return (1);
}
