/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 13:55:59 by hezhukov          #+#    #+#             */
/*   Updated: 2024/03/02 14:37:23 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Executes a command in the child process after forking.
 * @details
 * This function handles the execution in the child process.
 * It sets up:
	* 1. input and output redirections
 	* 2. closes the unused pipe end, and then
 	* 3. attempts to execute the command using execve.
 * If execve fails:
 	* 1. it reports the error
 	* 2. performs necessary cleanup
 	* 3. exits with failure status
 * this prevent the child process from continuing execution.
 * @param pnt Pointer to the main program data structure (t_data).
 * @param tab_cmd Pointer to the command table structure (t_tab_cmd).
 * @param fd_pipe Array of two integers representing the pipe file descriptors.
 */
void	execute_child_process(t_data *pnt, t_tab_cmd *tab_cmd)
{
	handle_redirection(tab_cmd->in_fd, STDIN_FILENO);
	handle_redirection(tab_cmd->out_fd, STDOUT_FILENO);
	close_pipe_end(pnt->fd_pipe, READ_END);
	pnt->fd_pipe[READ_END] = -1;
	set_mode(pnt, CHILD);
	if (execve(tab_cmd->cmd, tab_cmd->args, pnt->env) == -1)
	{
		error_out(pnt, tab_cmd->cmd, 1);
		total_clean(pnt);
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief Manages the forking and execution process for a command.
 *
 * Initiates the process of command execution by forking the current process.
 * In the child process:
	 * 1. it executes the command with potential redirections and env setup.
 * In the parent process:
	 * 1. it manages the closing pipe ends
	 * 2. updates the previous fds (fd_before) for subsequent commands.
 * It also handles cleanup after command execution.
 *
 * @param pnt Pointer to the main program data structure (t_data).
 * @param tab_cmd Pointer to the command structure (t_tab_cmd) to be executed.
 * @param i Index of the current command, used for managing fd_before.
 * @param fd_pipe Array containing the pipe file descriptors for IPC.
 */
void	command_execution(t_data *pnt, t_tab_cmd *tab_cmd, int i)
{
	tab_cmd->pid = fork();
	if (tab_cmd->pid < 0)
		return ((void)error_out(pnt, "fork", 1));
	if (tab_cmd->pid == 0)
		execute_child_process(pnt, tab_cmd);
	else
	{
		close_pipe_end(pnt->fd_pipe, WRITE_END);
		if (pnt->fd_before != -1)
			close(pnt->fd_before);
		if (pnt->cmdt_count - 1 != i)
			pnt->fd_before = pnt->fd_pipe[READ_END];
		else
		{
			close_pipe_end(pnt->fd_pipe, READ_END);
			pnt->fd_before = -1;
		}
		fd_cleaning(pnt, tab_cmd, i);
	}
}

/**
 * @brief Decides and initiates the execution of external commands.
 *
 * This function is called for commands that are not built-ins.
 * It first attempts to:
 * 1. find the executable in the system.
	 * If found, it proceeds with the command execution
 * 2. process by calling command_execution.
	 * If the command cannot be executed (not found or not executable)
	 * it attempts to handle any necessary redirections as a fallback.
 *
 * @param pnt Pointer to the main program data structure (t_data).
 * @param cmd Pointer to the command table structure (t_tab_cmd).
 * @param index Index of the current command being processed.
 * @param pip Pipe file descriptors array
 */
void	execute_external_command(t_data *pnt, t_tab_cmd *cmd, \
	int index)
{
	if (find_exec(pnt, cmd) == 0)
	{
		cmd->is_child_process++;
		command_execution(pnt, cmd, index);
	}
	else
		pipelines_redirect(pnt, index);
}

/**
 * @brief Orchestrates the execution of both built-in and external commands
 *
 * This is a key function for deciding how a command should be executed.
 * It first handles:
 * input and output redirections.
 	* If the command is a built-in, it executes it directly.
 * For external commands, it delegates to execute_external_command.
 * This setup ensures that all necessary pre-execution steps like:
 * - redirections and environment preparation
 *
 * @param pnt Pointer to the main data structure (t_data).
 * @param pip Pipe file descriptors array.
 * @param i Command index in the command table.
 */
void	execute_command(t_data *pnt, int i)
{
	if (input_output_redirect(pnt, &pnt->cmdt[i]) == 1 && \
		pipelines_redirect(pnt, i))
	{
		close(pnt->fd_pipe[1]);
		pnt->fd_pipe[1] = -1;
		return ;
	}
	change_fd_input_output(pnt, &pnt->cmdt[i], i);
	if (if_builtin(&pnt->cmdt[i]))
		execute_builtin(pnt, &pnt->cmdt[i], i);
	else
		execute_external_command(pnt, &pnt->cmdt[i], i);
}
