/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_fill_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 11:55:49 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/24 12:16:21 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * @brief Allocates and sets the command in the command table
 * @param data - The main data structure containing the command tables and tokens
 * @param command_index - The command index in the command table being populated
 * @param token_index - The index of the token that contains the command
 * @return int - Returns 0 on success or an error code if an error occurred
 */
int	set_command(t_data *data, int command_index, int token_index)
{
	data->cmdt[command_index].cmd = ft_strdup(data->tokens[token_index].value);
	if (!data->cmdt[command_index].cmd)
		return (error_out(data, "ft_strdup", 1) - 2);
	return (EXIT_SUCCESS);
}

/**
 * @brief Allocates and adds an argument to the current command's argument list
 * @param data - The main data structure containing the command tables and tokens
 * @param command_index - The command index in the command table being populated
 * @param argument_count - Tracks the index for adding the next argument.
 * @param token_index - The index of the token that contains the argument value
 * @return int - Returns 0 on success or an error code if an error occurred
 */
int	add_argument(t_data *data, int command_index, \
	int argument_count, int token_index)
{
	data->cmdt[command_index].args[argument_count] = \
		ft_strdup_fd(data->tokens[token_index].value);
	if (!data->cmdt[command_index].args[argument_count])
		return (error_out(data, "ft_strdup_fd", 1) - 2);
	return (0);
}

void	finalize_arguments(t_data *data, int command_index, int argument_count)
{
	if (data->cmdt[command_index].cmd != NULL)
		data->cmdt[command_index].args[argument_count] = NULL;
}

/**
 * @brief Processes a single token to determine if it should be:
 * 1. skipped
 * 2. set a command
 * 3. add an argument
 * @param data - The main data structure containing tokens and command tables
 * @param token_index - The index of the current token being processed
 * @param command_index - The index of the current command being populated
 * @param argument_count - Pointer to the current command's argument count.
 * @return 0  to indicate the end of command processing
 * @return 1  to continue
 * @return -1 for an error
 */
int	process_token(t_data *data, int token_index, \
	int command_index, int *argument_count)
{
	if (data->tokens[token_index].type == PIPE)
		return (0);
	if (check_arguments(data->tokens[token_index].type) && \
		(token_index == 0 || \
		check_if_redirection(data->tokens[token_index - 1].type)))
	{
		if (data->tokens[token_index].value[0] == '\0' && \
			data->tokens[token_index].type == WORD && \
			data->cmdt[command_index].num_args-- > 0)
			return (1);
		if (!data->cmdt[command_index].cmd)
			if (set_command(data, command_index, token_index) != 0)
				return (-1);
		if (add_argument(data, command_index, \
			*argument_count, token_index) != 0)
			return (-1);
		(*argument_count)++;
	}
	return (1);
}

/**
 * @brief Parses and fill a command structure with commands and arguments
 * @param data - main data structure containing tokens and command tables
 * @param command_index - Current command index command structure being populated
 * @param token_index - Initial token index for current command processing.
 * @return token_index of the last token processed for potential continuation.
 */
int	parse_and_fill_command(t_data *data, int command_index, int token_index)
{
	int	argument_count;
	int	process_result;

	token_index += 1;
	argument_count = 0;
	while (token_index < data->count_token)
	{
		process_result = process_token(data, token_index, \
			command_index, &argument_count);
		if (process_result == 0)
			break ;
		else if (process_result == -1)
			return (token_index - 1);
		token_index++;
	}
	finalize_arguments(data, command_index, argument_count);
	return (token_index);
}
