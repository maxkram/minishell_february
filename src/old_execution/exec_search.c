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
			ft_printf_fd(2, "minishell: %s: Is a directory\n", tab_cmd->cmd);
			pntr->code_exit = 126;
			closedir(directory);
		}
		else if (access(tab_cmd->cmd, X_OK) == 0)
			return (0);
		else
		{
			ft_printf_fd(2, "minishell: %s: No permission\n",
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

//"search_in_path", is responsible for searching the provided command in the directories specified in the PATH environment variable. It iterates through each directory in the PATH, concatenates the directory path with the command, and checks if the resulting path is executable. If an executable path is found, it updates the "cmd_table->cmd" field with the full path to the executable.
/**
 * @leaks
 * fix the memory leaks
 * [ ] 89 bytes in 5 blocks are definitely lost in loss record 35 of 96
 * 		==1069==    by 0x10000B244: ft_strdup_fd (in ./minishell)
 *		==1069==    by 0x100009104: path_searching (exec_search.c:76)
 * [ ] 94 bytes in 5 blocks are definitely lost in loss record 36 of 96
 *		==1069==    by 0x10000B322: ft_strjoin (in ./minishell)
 *		==1069==    by 0x100009143: path_searching (exec_search.c:79)
*/
int path_searching(t_data *pnt, t_tab_cmd *tab_cmd, int i)
{
    char *ret;
    char *temp;

    while (pnt->path && pnt->path[++i])
    {
        if (tab_cmd->cmd[0] == '\0')
            break;
        temp = ft_strdup_fd(pnt->path[i]);
        if (!temp)
            return (error_out(pnt, "ft_strdup", 1) + 1);
        printf("Allocated temp (strdup): %s\n", temp); // Log allocation

        temp = ft_strjoin(temp, "/");
        if (!temp)
            return (error_out(pnt, "ft_strjoin", 1) + 1);
        printf("Reallocated temp (strjoin): %s\n", temp); // Log reallocation

        ret = ft_strjoin(temp, tab_cmd->cmd);
        if (!ret)
            return (error_out(pnt, "ft_strjoin", 1) + 1);
        printf("Allocated ret (strjoin): %s\n", ret); // Log allocation

        if (access(ret, X_OK) == 0)
        {
            // temp = tab_cmd->cmd; // This line would cause a leak if uncommented
            printf("Command found: %s\n", ret); // Indicate successful command find
            tab_cmd->cmd = ret;
            free(temp); // Free temp after reassigning tab_cmd->cmd to ret
            printf("Freed temp after command found\n");
            return 0;
        }
        free(ret);
        printf("Freed ret\n"); // Log freeing ret
        free(temp); // This correctly frees temp
        printf("Freed temp\n"); // Log freeing temp
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
			ft_printf_fd(2, "minishell: %s: No permission\n",
				tab_cmd->cmd);
			pntr->code_exit = 126;
			return (0);
		}
		free(result);
	}
	return (is_command(pntr, tab_cmd));
}

//the function is responsible for locating an executable binary in the system
//and determining whether it is available for execution (127 - command not found)

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
