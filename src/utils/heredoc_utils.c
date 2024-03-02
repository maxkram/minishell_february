/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 17:47:21 by device            #+#    #+#             */
/*   Updated: 2024/03/02 14:37:43 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	close_and_free(char *buffer, int fd, int return_value)
{
	close(fd);
	if (buffer != NULL)
		free(buffer);
	return (return_value);
}

int	using_dollar(char *letter, char **result, t_data *pnt, int e)
{
	const int	length = length_of_variable(letter);
	char		*buffer;
	char		*sign;

	if (e && length == 1)
		return (*result = ft_strdup_fd(""), length);
	if (length == 1)
		return (*result = ft_strdup_fd("$"), length);
	sign = ft_substr(letter, 1, length - 1);
	if (sign == NULL)
		return (length);
	buffer = value_of_variable(pnt, sign);
	free(sign);
	if (buffer == NULL)
		*result = ft_strdup_fd("");
	else
		*result = ft_strdup_fd(buffer);
	free(buffer);
	return (length);
}

static char	*broaden_local_token(t_data *pnt, char *letter)
{
	char	*result;
	char	*buffer;
	char	*buffer_result;
	char	*buffer_value;

	result = NULL;
	buffer_value = letter;
	if (!*letter)
		return (letter);
	while (*letter)
	{
		buffer = NULL;
		if (*letter == '$')
			letter += using_dollar(letter, &buffer, pnt, 0);
		else
			letter += substring_concatenation(result, &buffer);
		if (buffer == NULL)
			return (free(result), NULL);
		buffer_result = ft_strcat(result, buffer);
		(free(buffer), free(result));
		if (buffer_result == NULL)
			return (NULL);
		result = buffer_result;
	}
	return (free(buffer_value), result);
}

static int	process_buffer(t_data *pnt, char **buffer, int fd, char *delimiter)
{
	if (ft_strcmp(*buffer, delimiter) == 0)
	{
		free(*buffer);
		return (0);
	}
	if ((*buffer)[0] == '$')
		*buffer = broaden_local_token(pnt, *buffer);
	ft_putendl_fd(*buffer, fd);
	free(*buffer);
	return (1);
}

int	in_to_fd(t_data *pnt, int fd, char *delimiter)
{
	char	*buffer;

	set_mode(pnt, MULTILINE);
	while (1)
	{
		buffer = readline("> ");
		if (buffer == NULL || g_global_signal == 1)
			return (close_and_free(buffer, fd, 1));
		if (process_buffer(pnt, &buffer, fd, delimiter) == 0)
			break ;
	}
	set_mode(pnt, NON_INTERACT);
	return (0);
}
