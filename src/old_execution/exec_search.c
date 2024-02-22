#include "minishell.h"

//The function handles cases where the PATH environment variable is not set and checks for the existence of the command in the current directory, updating the command and handling errors accordingly

int	is_command(t_data *pntr, t_tab_cmd *tab_cmd)
{
	char	*result;
	char	*temporary;

	if (pntr->path != NULL)
		return (1);
	temporary = ft_strdup_fd("./");
	if (!temporary)
		return(error_out(pntr, "ft_strdup", 1) + 1);
	result = ft_strjoin(temporary, tab_cmd->cmd);
	if (!result)
		return (error_out(pntr, "ft_strjoin", 1) + 1);
	if (access(result, 0) == 0)
	{
		temporary = tab_cmd->cmd;
		tab_cmd->cmd = result;
		free(temporary);
				ft_printf_fd(2, "minishell: %s: Permission denied\n", tab_cmd->cmd);
		pntr->code_exit = 126;
		return (0);
	}
	free(result);
	return (1);
}

//function checks if a specified command or file path is valid for execution. It checks whether the file exists, whether it is a directory, and whether it is executable. It prints appropriate error messages and sets the exit code accordingly. The function returns 0 for success and 1 for failure.

int check_valid_execution(t_tab_cmd *tab_cmd, t_data *pntr)
{
	DIR *directory;

	if (access(tab_cmd->cmd, 0) == 0)
	{
		directory = opendir(tab_cmd->cmd);
		if (directory)
		{
			ft_printf_fd(2, "minishell: %s: is a directory\n", tab_cmd->cmd);
			pntr->code_exit = 126;
			closedir(directory);
		}
		else if (access(tab_cmd->cmd, X_OK) == 0)
			return (0);
		else
		{
			ft_printf_fd(2, "minishell: %s: Permission denied\n",
				tab_cmd->cmd);
			pntr->code_exit = 126;
		}
		return (1);
	}
	ft_printf_fd(2, "minishell: %s: No such file or directory\n",
		tab_cmd->cmd);
	pntr->code_exit = 127;
	return (1);
}

/**
 * @brief Search for the command in the PATH environment variable
 * @details
 * 1. Iterate through each directory in the PATH environment variable
 * 2. Concatenate the directory path with the command
 * 3. Check if the resulting path is executable
 * 4. If an executable path is found, update the "cmd_table->cmd" field with the full path to the executable
 * @param pnt The main data struct
 * @param tab_cmd The command table struct
 * @param i The index of the current path in the PATH environment variable
 * @return 0 for success, 1 for failure, 2 for no path found
*/
int path_searching(t_data *pnt, t_tab_cmd *tab_cmd, int i)
{
    char *path_to_check;
    char *joined_path_with_slash;
    char *full_path;

    while (pnt->path && pnt->path[++i])
    {
        if (tab_cmd->cmd[0] == '\0')
            break;
        path_to_check = ft_strdup(pnt->path[i]);
        if (!path_to_check)
            return (error_out(pnt, "Memory allocation failed for path duplication", 1) + 1);
        joined_path_with_slash = ft_strjoin(path_to_check, "/");
        free(path_to_check);
        if (!joined_path_with_slash)
            return (error_out(pnt, "Memory allocation failed for joining path with slash", 1) + 1);
        full_path = ft_strjoin(joined_path_with_slash, tab_cmd->cmd);
        free(joined_path_with_slash);
        if (!full_path)
            return (error_out(pnt, "Memory allocation failed for joining command to path", 1) + 1);
        if (access(full_path, X_OK) == 0)
        {
            free(tab_cmd->cmd);
            tab_cmd->cmd = full_path;
            return 0;
        }
        else
        {
            free(full_path);
        }
    }
    return (find_path(pnt, tab_cmd));
}



//'search_if_exist' is responsible for checking if a given command exists in the directories listed in the PATH environment variable

int	is_exist(t_data *pntr, t_tab_cmd *tab_cmd, int i)
{
	char	*result;
	char	*temporary;

	while (pntr->path && pntr->path[++i])
	{
		temporary = ft_strdup_fd(pntr->path[i]);
		if (!temporary)
			return (error_out(pntr, "ft_strdup", 1) + 1);
		result = ft_strjoin(temporary, tab_cmd->cmd);
		if (!result)
			return(error_out(pntr, "ft_strjoin", 1) + 1);
		if (access(result, 0) == 0)
		{
			temporary = tab_cmd->cmd;
			tab_cmd->cmd = result;
			free(temporary);
			ft_printf_fd(2, "minishell: %s: Permission denied\n",
				tab_cmd->cmd);
			pntr->code_exit = 126;
			return (0);
		}
		free(temporary); // added to fix the memory leak
		free(result);
	}
	return (is_command(pntr, tab_cmd));
}

//the function is responsible for locating an executable binary in the system
//and determining whether it is available for execution (127 - command not found)

/**
 * @brief Find the executable for the command
 * @details
 * 1. If the command is empty, return 1
 * 2. If the command is a relative or absolute path, check if it is valid for execution
 * 3. Search for the command in the PATH environment variable
 * 4. If the command is found, update the "cmd_table->cmd" field with the full path to the executable
 * 5. If the command is not found, print an error message and set the exit code to 127
 * @param pntr The main data struct
 * @param tab_cmd The command table struct
 * @return 0 for success, 1 for failure
 * @changes commented out the printf statement because it does not pass the tester ./tester extras
 *
*/
int find_exec(t_data *pntr, t_tab_cmd *tab_cmd)
{
	int i;
	int result;

	i = -1;
	if (!tab_cmd->cmd)
		return (1);
	if (tab_cmd->cmd[0] == '\0')
	{
		ft_printf_fd(2, "minishell: %s: command not found\n", tab_cmd->cmd);
		pntr->code_exit = 127;
		return (1);
	}
	if (tab_cmd->cmd[0] == '.' || ft_strchr(tab_cmd->cmd, '/') != 0)
		return (check_valid_execution(tab_cmd, pntr));
	result = path_searching(pntr, tab_cmd, i);
	// printf("Result of path_searching: %d\n", result); // Log result of path_searching
	if (result == 0)
		return (0);
	else if (result == 2)
		return (1);
	if (is_exist(pntr, tab_cmd, i) == 1)
	{
		ft_printf_fd(2, "minishell: %s: command not found\n", tab_cmd->cmd);
		pntr->code_exit = 127;
	}
	return (1);
}
