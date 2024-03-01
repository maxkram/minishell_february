/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 13:56:30 by hezhukov          #+#    #+#             */
/*   Updated: 2024/03/01 13:37:55 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirects_cmd_tab(t_data *pnt, t_tab_cmd *tab_cmd, int i)
{
	if (tab_cmd->redirections[i].type == REDIRECT_OUT)
	{
		if (tab_cmd->file_out != -1)
			close(tab_cmd->file_out);
		tab_cmd->file_out = open(tab_cmd->redirections[i].value,
				O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0644);
		if (tab_cmd->file_out == -1)
			return (error_out(pnt, tab_cmd->redirections[i].value, 1));
	}
	else if (tab_cmd->redirections[i].type == REDIRECT_APPEND)
	{
		if (tab_cmd->file_out != -1)
			close(tab_cmd->file_out);
		tab_cmd->file_out = open(tab_cmd->redirections[i].value,
				O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0644);
		if (tab_cmd->file_out == -1)
			return (error_out(pnt, tab_cmd->redirections[i].value, 1));
	}
	else if (tab_cmd->redirections[i].type == REDIRECT_MULTILINE)
		if (create_heredoc(pnt, tab_cmd, i) == 1)
			return (1);
	return (EXIT_SUCCESS);
}

/**
 * @brief Handles input redirection ('<')
 *
 * Opens the file specified and sets it as the input source for the command.
 * If a previous file was opened for input redirection,
 * it is closed before opening the new file.
 * @param pnt Pointer to the main data structure.
 * @param tab_cmd Pointer to the current command structure being processed.
 * @param i Index of the current redirection in the command's redirection array.
 * @return 0 on successful redirection setup, or an error code on failure.
 */
int	handle_input_redirection(t_data *pnt, t_tab_cmd *tab_cmd, int i)
{
	if (tab_cmd->file_in != -1)
		close(tab_cmd->file_in);
	tab_cmd->file_in = open(tab_cmd->redirections[i].value, \
		O_RDONLY | O_CLOEXEC);
	if (tab_cmd->file_in == -1)
		return (error_out(pnt, tab_cmd->redirections[i].value, 1));
	return (EXIT_SUCCESS);
}

/**
 * @brief Processes all input and output redirections for a command.
 *
 * Iterates through each redirection in the command and handles it accordingly.
 * Input redirections are handled directly.
 * Other redirection types are managed by a separate function.
 * Invalid redirections, such as those without a target,
 * result in an error message and termination of the redirection process.
 *
 * @param pnt Pointer to the main data structure.
 * @param tab_cmd Pointer to the command table structure.
 * @return 0 if all redirections are successfully processed
 * @return 1 if an error occurs during processing.
 */
int	input_output_redirect(t_data *pnt, t_tab_cmd *tab_cmd)
{
	int	i;

	i = -1;
	while (tab_cmd->num_redirections > ++i)
	{
		if (tab_cmd->redirections[i].type != REDIRECT_MULTILINE && \
			tab_cmd->redirections[i].no_space == 2)
		{
			ft_putstr_fd("minishell: redirect to nowhere\n", 2);
			pnt->code_exit = 1;
			return (1);
		}
		if (tab_cmd->redirections[i].type == REDIRECT_IN)
		{
			if (handle_input_redirection(pnt, tab_cmd, i) != 0)
				return (EXIT_FAILURE);
		}
		else
		{
			if (redirects_cmd_tab(pnt, tab_cmd, i) == 1)
				return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}
