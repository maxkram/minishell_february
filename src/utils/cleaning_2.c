/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:06 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/24 00:30:53 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// @todo test with valgrind this new function
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

// @todo I refactored this new function.
// void	cmdt_cleaning(t_data *pntr)
// {
// 	if (pntr->cmdt)
// 	{
// 		while (0 < pntr->cmdt_count--)
// 		{
// 			if (pntr->cmdt[pntr->cmdt_count].args)
// 				while (0 < pntr->cmdt[pntr->cmdt_count].num_args--)
// 					free(pntr->cmdt[pntr->cmdt_count].args[pntr->cmdt[pntr->cmdt_count].num_args]);
// 			if (pntr->cmdt[pntr->cmdt_count].args)
// 				free(pntr->cmdt[pntr->cmdt_count].args);
// 			pntr->cmdt[pntr->cmdt_count].args = NULL;
// 			if (pntr->cmdt[pntr->cmdt_count].redirections)
// 				while (0 < pntr->cmdt[pntr->cmdt_count].num_redirections--)
// 					free(pntr->cmdt[pntr->cmdt_count].redirections[pntr->cmdt[pntr->cmdt_count].num_redirections].value);
// 			if (pntr->cmdt[pntr->cmdt_count].redirections)
// 				free(pntr->cmdt[pntr->cmdt_count].redirections);
// 			pntr->cmdt[pntr->cmdt_count].redirections = NULL;
// 			if (pntr->cmdt[pntr->cmdt_count].cmd)
// 				free(pntr->cmdt[pntr->cmdt_count].cmd);
// 			pntr->cmdt[pntr->cmdt_count].cmd = NULL;
// 		}
// 		free(pntr->cmdt);
// 		pntr->cmdt = NULL;
// 	}
// }

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

int	clean_tokens(t_token *tokens, int max, t_data *pnt)
{
	while (pnt->count_token > ++max)
	{
		if (tokens[max].value)
		{
			free(tokens[max].value);
			tokens[max].value = NULL;
		}
	}
	free(tokens);
	tokens = NULL;
	return (1);
}
