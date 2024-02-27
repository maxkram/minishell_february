/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:06 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/27 14:06:48 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	clean_tokens(t_token *tokens, int max, t_data *pnt)
{
	while (pnt->count_token > ++max)
	{
		if (tokens[max].value)
		{
			free(tokens[max].value);
			tokens[max].value = NULL;
		}
	}
	free(tokens);
	tokens = NULL;
	return (1);
}

void close_all_fds(t_data *data) {
    // Loop through all command table entries
    for (int i = 0; i < data->cmdt_count; i++) {
        t_tab_cmd *cmd = &(data->cmdt[i]);
        if (cmd->in_fd != STDIN_FILENO && cmd->in_fd != STDOUT_FILENO && cmd->in_fd != STDERR_FILENO) {
            close(cmd->in_fd); // Close the input file descriptor
        }
        if (cmd->out_fd != STDIN_FILENO && cmd->out_fd != STDOUT_FILENO && cmd->out_fd != STDERR_FILENO) {
            close(cmd->out_fd); // Close the output file descriptor
        }
    }
}
