/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:54:36 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/28 19:41:29 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*construct_full_path(char *path, char *cmd)
{
	char	*path_with_slash;
	char	*full_path;

	path_with_slash = ft_strjoin(path, "/");
	if (!path_with_slash)
		return (NULL);
	full_path = ft_strjoin(path_with_slash, cmd);
	free(path_with_slash);
	return (full_path);
}

static int	handle_path_found(t_tab_cmd *tab_cmd, char *full_path)
{
	free(tab_cmd->cmd);
	tab_cmd->cmd = full_path;
	return (EXIT_SUCCESS);
}

static int	cleanup_and_return(char *msg, char *to_free, int return_value)
{
	if (to_free)
		free(to_free);
	ft_putstr_fd(msg, STDERR_FILENO);
	return (return_value);
}

int	path_searching(t_data *pnt, t_tab_cmd *tab_cmd)
{
	char	*full_path;
	int		i;

	i = -1;
	while (pnt->path && pnt->path[++i])
	{
		if (tab_cmd->cmd[0] == '\0')
			break ;
		full_path = construct_full_path(pnt->path[i], tab_cmd->cmd);
		if (!full_path)
			return (cleanup_and_return("Memory allocation failed", NULL, 1));
		if (access(full_path, X_OK) == 0)
			return (handle_path_found(tab_cmd, full_path));
		free(full_path);
	}
	return (find_path(pnt, tab_cmd));
}
