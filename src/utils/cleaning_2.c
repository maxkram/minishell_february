/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:06 by hezhukov          #+#    #+#             */
/*   Updated: 2024/03/01 18:19:16 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmdt_resources(t_tab_cmd *cmd)
{
	if (cmd->args)
	{
		while (cmd->num_args--)
			free(cmd->args[cmd->num_args]);
		free(cmd->args);
		cmd->args = NULL;
	}
	if (cmd->redirections)
	{
		while (cmd->num_redirections--)
			free(cmd->redirections[cmd->num_redirections].value);
		free(cmd->redirections);
		cmd->redirections = NULL;
	}
	if (cmd->cmd)
	{
		free(cmd->cmd);
		cmd->cmd = NULL;
	}
}

void	cmdt_cleaning(t_data *pnt)
{
	if (pnt->cmdt)
	{
		while (pnt->cmdt_count--)
		{
			free_cmdt_resources(&pnt->cmdt[pnt->cmdt_count]);
		}
		free(pnt->cmdt);
		pnt->cmdt = NULL;
	}
}

void	pntr_cleaning(t_data *pnt)
{
	int	i;

	if (pnt->tokens)
	{
		while (pnt->count_token > 0)
			free(pnt->tokens[--pnt->count_token].value);
		free(pnt->tokens);
		pnt->tokens = NULL;
	}
	cmdt_cleaning(pnt);
	if (pnt->input)
		free(pnt->input);
	pnt->input = NULL;
	i = 0;
	if (pnt->path)
	{
		while (pnt->path[i])
			free(pnt->path[i++]);
		free(pnt->path);
	}
	pnt->path = NULL;
}

void	fd_cleaning(t_data *pnt, t_tab_cmd *tab_cmd, int i)
{
	if (tab_cmd->out_fd != -1)
	{
		close(tab_cmd->out_fd);
		tab_cmd->out_fd = -1;
	}
	if (tab_cmd->in_fd != -1)
	{
		close(tab_cmd->in_fd);
		tab_cmd->in_fd = -1;
	}
	if (pnt->cmdt[i].last_multiline)
	{
		unlink(pnt->cmdt[i].last_multiline);
		free(pnt->cmdt[i].last_multiline);
	}
}

void	total_clean(t_data *pnt)
{
	clean_double_pointer(pnt->env);
	pntr_cleaning(pnt);
	if (pnt->first_stdout != -1)
	{
		close(pnt->first_stdout);
		pnt->first_stdout = -1;
	}
	if (pnt->first_stdin != -1)
	{
		close(pnt->first_stdin);
		pnt->first_stdin = -1;
	}
	if (pnt->fd_pipe[0] != -1)
	{
		close(pnt->fd_pipe[0]);
		pnt->fd_pipe[0] = -1;
	}
	if (pnt->fd_pipe[1] != -1)
	{
		close(pnt->fd_pipe[1]);
		pnt->fd_pipe[1] = -1;
	}
}
