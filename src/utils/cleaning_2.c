/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:06 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/24 10:54:44 by hezhukov         ###   ########.fr       */
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

void	cmdt_cleaning(t_data *pntr)
{
	if (pntr->cmdt)
	{
		while (pntr->cmdt_count--)
		{
			free_cmdt_resources(&pntr->cmdt[pntr->cmdt_count]);
		}
		free(pntr->cmdt);
		pntr->cmdt = NULL;
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

void	fd_cleaning(t_data *pntr, t_tab_cmd *tab_cmd, int i)
{
	if (tab_cmd->out_fd != -1)
		close(tab_cmd->out_fd);
	else if (tab_cmd->in_fd != -1)
		close(tab_cmd->in_fd);
	if (pntr->cmdt[i].last_multiline)
	{
		unlink(pntr->cmdt[i].last_multiline);
		free(pntr->cmdt[i].last_multiline);
	}
}

void	total_clean(t_data *pntr)
{
	clean_double_pointer(pntr->env);
	pntr_cleaning(pntr);
	close(pntr->first_stdout);
	close(pntr->first_stdin);
}
