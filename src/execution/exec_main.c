/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 13:54:35 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/27 19:58:51 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_pipes(int *fd_pipe, t_data *pnt)
{
	if (pipe(fd_pipe) == -1)
	{
		error_out(pnt, "pipe", 1);
		return (-1);
	}
	return (0);
}

void	wait_for_childs(t_data *pnt)
{
	int	i;
	int	status;

	i = -1;
	while (pnt->cmdt_count > ++i)
	{
		if (pnt->cmdt[i].is_child_process == 1)
			waitpid(pnt->cmdt[i].pid, &status, 0);
	}
	if (pnt->cmdt[i - 1].is_child_process == 1)
	{
		if (WIFSIGNALED(status))
			pnt->code_exit = WTERMSIG(status) + 128;
		else if (WIFEXITED(status))
			pnt->code_exit = WEXITSTATUS(status);
	}
}

void	execution(t_data *pnt)
{
	int	i;
	int	fd_pipe[2];

	i = -1;
	pnt->fd_before = -1;
	while (++i < pnt->cmdt_count)
	{
		if (setup_pipes(fd_pipe, pnt) == -1)
			return ((void)error_out(pnt, "pipe", 1));
		execute_command(pnt, fd_pipe, i);
	}
	wait_for_childs(pnt);
}
