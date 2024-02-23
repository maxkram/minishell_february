/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiline.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:36 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/22 19:13:42 by hezhukov         ###   ########.fr       */
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
	file_name = ft_strcat(".tmp_hdoc", with_itoa);
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

static int	input_to_file_descriptor(t_data *pnt, int fd, char *delimiter)
{
	char			*string;
	static int		i;

	i = 0;
	set_mode(pnt, MULTILINE);
	while (1)
	{
		string = readline("> ");
		if (global_signal == 1)
			return (free(string), 1);
		if (string == NULL && ft_printf_fd(STDERR_FILENO, "minishell: warning: here-document at line %d delimited by end-of-file(wanted `%s')\n", i, delimiter))
			break ;
		if (ft_strcmp(string, delimiter) == 0)
			break ;
		string = broaden_local_token(pnt, string);
		if (!string)
			return (close(fd), 2);
		ft_putendl_fd(string, fd);
		free(string);
		i++;
	}
	set_mode(pnt, NON_INTERACT);
	if (string != NULL)
		free(string);
	return (0);
}

int	create_heredoc(t_data *pnt, t_tab_cmd *tab_cmd, int i)
{
	char	*object;
	int		status;
	int		file_descriptor;

	object = name_create_multiline(i);
	if (!object)
		return (error_out(pnt, "malloc issue", 1), 1);
	file_descriptor = open(object, O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (file_descriptor < 0)
		return (free(object), error_out(pnt, "minishell: open: ", 1));
	status = input_to_file_descriptor(pnt, file_descriptor, tab_cmd->redirections[i].value);
	if (status == 1 || status == 2)
		return (unlink(object), free(object), 1);
	close(file_descriptor);
	if (tab_cmd->redirections[i].no_space != 3)
		unlink(object);
	else
	{
		tab_cmd->file_in = open(object, O_RDONLY);
		if (tab_cmd->file_in == -1)
			return (free(object), error_out(pnt, "minishell: open: ", 1));
		tab_cmd->last_multiline = ft_strdup_fd(object);
	}
	free(object);
	return (0);
}
