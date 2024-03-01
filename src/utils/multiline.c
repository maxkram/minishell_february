/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiline.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:36 by hezhukov          #+#    #+#             */
/*   Updated: 2024/03/01 18:03:59 by device           ###   ########.fr       */
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
	file_name = ft_strcat("/tmp/.tmp_hdoc", with_itoa);
	free(with_itoa);
	return (file_name);
}

static	int	open_temp_file(char *object, t_data *pnt)
{
	int	fd;

	fd = open(object, O_CREAT | O_TRUNC | O_RDWR | O_CLOEXEC, 0666);
	if (fd < 0)
	{
		free(object);
		error_out(pnt, "minishell: open: ", 1);
	}
	return (fd);
}

int	create_heredoc(t_data *pnt, t_tab_cmd *tab_cmd, int i)
{
	char	*object;
	int		stat;
	int		fd;

	object = name_create_multiline(i);
	if (!object)
		return (error_out(pnt, "malloc issue", 1), 1);
	fd = open_temp_file(object, pnt);
	if (fd < 0)
		return (free(object), error_out(pnt, "minishell: open: ", 1));
	stat = in_to_fd(pnt, fd, tab_cmd->redirections[i].value);
	if (stat == 1 || stat == 2)
		return (unlink(object), free(object), close(fd), 1);
	if (tab_cmd->redirections[i].no_space != 3)
		unlink(object);
	else
	{
		tab_cmd->file_in = open(object, O_RDONLY);
		if (tab_cmd->file_in == -1)
			return (free(object), error_out(pnt, "minishell: open: ", 1));
		tab_cmd->last_multiline = ft_strdup_fd(object);
	}
	close(fd);
	free(object);
	return (EXIT_SUCCESS);
}
