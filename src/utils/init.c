/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:30 by hezhukov          #+#    #+#             */
/*   Updated: 2024/03/02 12:24:06 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_start_file_desc(t_data *pnt)
{
	pnt->first_stdout = dup(STDOUT_FILENO);
	if (pnt->first_stdout == -1)
		fd_exit(pnt, 1);
	pnt->first_stdin = dup(STDIN_FILENO);
	if (pnt->first_stdin == -1)
		fd_exit(pnt, 1);
}

char	**string_to_array(char **source)
{
	char	**copy;
	int		i;

	copy = (char **)ft_calloc(sizeof(char *), ft_split_length(source) + 1);
	if (copy == NULL)
		return (NULL);
	i = 0;
	while (source[i] != NULL)
	{
		copy[i] = ft_strdup_fd(source[i]);
		if (copy[i] == NULL)
			return (free_string_array(copy), NULL);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

void	init_pnt(t_data *pnt, char **env)
{
	char	**copy_env;

	pnt->env = NULL;
	pnt->path = NULL;
	pnt->input = NULL;
	pnt->tokens = NULL;
	pnt->count_token = 0;
	pnt->max_token = 1;
	pnt->code_exit = 0;
	pnt->cmdt = NULL;
	pnt->cmdt_count = 0;
	pnt->mode = INTERACT;
	init_start_file_desc(pnt);
	copy_env = string_to_array(env);
	if (copy_env == NULL)
		fd_exit(pnt, 1);
	pnt->env = copy_env;
	if (increase_shlvl(pnt) == 1)
		fd_exit(pnt, 0);
	pnt->n_pipes = 0;
}
