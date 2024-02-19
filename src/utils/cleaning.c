#include "minishell.h"

//cleans memory allocated by the command table (pntr->cmdt)
void	cmdt_cleaning(t_data *pntr)
{
	if (pntr->cmdt)
	{
		while (0 < pntr->cmdt_count--)
		{
			if (pntr->cmdt[pntr->cmdt_count].args)
				while (0 < pntr->cmdt[pntr->cmdt_count].num_args--)
					free(pntr->cmdt[pntr->cmdt_count].args[pntr->cmdt[pntr->cmdt_count].num_args]);
			if (pntr->cmdt[pntr->cmdt_count].args)
				free(pntr->cmdt[pntr->cmdt_count].args);
			pntr->cmdt[pntr->cmdt_count].args = NULL;
			if (pntr->cmdt[pntr->cmdt_count].redirections)
				while (0 < pntr->cmdt[pntr->cmdt_count].num_redirections--)
					free(pntr->cmdt[pntr->cmdt_count].redirections[pntr->cmdt[pntr->cmdt_count].num_redirections].value);
			if (pntr->cmdt[pntr->cmdt_count].redirections)
				free(pntr->cmdt[pntr->cmdt_count].redirections);
			pntr->cmdt[pntr->cmdt_count].redirections = NULL;
			if (pntr->cmdt[pntr->cmdt_count].cmd)
				free(pntr->cmdt[pntr->cmdt_count].cmd);
			pntr->cmdt[pntr->cmdt_count].cmd = NULL;
		}
		free(pntr->cmdt);
		pntr->cmdt = NULL;
	}
}

//the function is responsible for freeing memory allocated for various components of the t_data structure
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

//function for safely freeing the memory associated with an array of strings

// void	double_pntr_cleaning(char **pntr)
// {
// 	// if (!pntr)
// 	// 	return ;
// 	// while (*pntr)
// 	// {
// 	// 	free(*pntr);
// 	// 	*pntr++ = NULL;
// 	// }
// 	// free(pntr);
// 	// pntr = NULL;
// 	int	i;

// 	i = 0;
// 	if (pntr)
// 	{
// 		while(pntr[i])
// 		{
// 			free(pntr[i]);
// 			pntr[i] = NULL;
// 			i++;
// 		}
// 		free(pntr);
// 		pntr = NULL;
// 	}
// }

//the function is responsible for cleaning up file descriptors and resources associated with output redirection and here documents

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

//the function designed to release resources and perform cleanup tasks before the program exits. It frees memory, closes file descriptors, and delegates additional cleanup to the clean_stuff function.

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
void cleanup_tokens(t_data *data) {
    if (data->tokens) {
        for (int i = 0; i < data->count_token; i++) {
            if (data->tokens[i].value) {
                free(data->tokens[i].value); // Free the value of each token
                data->tokens[i].value = NULL; // Avoid dangling pointer by setting to NULL
            }
        }
        free(data->tokens); // Free the entire tokens array
        data->tokens = NULL; // Avoid dangling pointer by setting to NULL
        data->count_token = 0; // Reset token count to 0
    }
}
