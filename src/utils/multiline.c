/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiline.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:36 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/29 15:58:57 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*name_create_multiline(int i)
{
	char	*with_itoa;
	char	*file_name;

	with_itoa = ft_itoa(i);
	if (with_itoa == NULL)
		return (NULL);
	// file_name = ft_strcat(".tmp_hdoc", with_itoa);
	file_name = ft_strcat("/tmp/.tmp_hdoc", with_itoa);
	free(with_itoa);
	return (file_name);
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

// static int	in_to_fd(t_data *pnt, int fd, char *delimiter)
// {
// 	char			*buffer;

<<<<<<< HEAD
// 	set_mode(pnt, MULTILINE);
// 	while (1)
// 	{
// 		buffer = readline("> ");
// 		if (g_global_signal == 1)
// 			return (free(buffer), 1);
// 		if (ft_strcmp(buffer, delimiter) == 0)
// 			break ;
// 		if (buffer[0] == '$')
// 			buffer = broaden_local_token(pnt, buffer);
// 		write(fd, buffer, ft_strlen(buffer));
// 		write(fd, "\n", 1);
// 		free(buffer);
// 	}
// 	set_mode(pnt, NON_INTERACT);
// 	if (buffer != NULL)
// 		free(buffer);
// 	return (0);
// }

static int in_to_fd(t_data *pnt, int fd, char *delimiter) {
    char *buffer;

    set_mode(pnt, MULTILINE);
    while (1) {
        buffer = readline("> ");
        if (buffer == NULL || g_global_signal == 1) { // Check if buffer is NULL or if a signal was received
            close(fd);
            if (buffer != NULL) {
                free(buffer); // Free buffer if it's not NULL
            }
            return 1;
        }
        if (ft_strcmp(buffer, delimiter) == 0) {
            free(buffer); // Free buffer to prevent memory leak
            break;
        }
        if (buffer[0] == '$') {
            char *temp = buffer;
            buffer = broaden_local_token(pnt, buffer);
            free(temp); // Ensure the original buffer is freed if it's replaced
        }
        write(fd, buffer, ft_strlen(buffer));
        write(fd, "\n", 1);
        free(buffer);
    }
    set_mode(pnt, NON_INTERACT);
    // Removed redundant NULL check and free for buffer here, as buffer should always be NULL at this point
    return 0;
=======
	set_mode(pnt, MULTILINE);
	while (1)
	{
		buffer = readline("> ");
		if (g_global_signal == 1)
			return (free(buffer), 1);
		if (ft_strcmp(buffer, delimiter) == 0)
			break ;
		if (buffer[0] == '$')
			buffer = broaden_local_token(pnt, buffer);
		write(fd, buffer, ft_strlen(buffer));
		write(fd, "\n", 1);
		free(buffer);
	}
	set_mode(pnt, NON_INTERACT);
	if (buffer != NULL)
		free(buffer);
	return (EXIT_SUCCESS);
>>>>>>> main
}


int	create_heredoc(t_data *pnt, t_tab_cmd *tab_cmd, int i)
{
	char	*object;
	int		stat;
	int		file_descriptor;

	object = name_create_multiline(i);
	if (!object)
		return (error_out(pnt, "malloc issue", 1), 1);
	file_descriptor = open(object, O_CREAT | O_TRUNC | O_RDWR | O_CLOEXEC, 0666);
	if (file_descriptor < 0)
		return (free(object), error_out(pnt, "minishell: open: ", 1));
	stat = in_to_fd(pnt, file_descriptor, tab_cmd->redirections[i].value);
	if (stat == 1 || stat == 2)
	{
		return (unlink(object), free(object), 1);
		close(file_descriptor);
	}
	if (tab_cmd->redirections[i].no_space != 3)
		unlink(object);
	else
	{
		tab_cmd->file_in = open(object, O_RDONLY);
		if (tab_cmd->file_in == -1)
			return (free(object), error_out(pnt, "minishell: open: ", 1));
		tab_cmd->last_multiline = ft_strdup_fd(object);
	}
	close(file_descriptor);
	free(object);
	return (EXIT_SUCCESS);
}
