#include "minishell.h"

// The function creates a child process to execute the specified command,
//handling input and output redirection, executes the commands using
//execve and managing file descriptors in the parent process.

#include <stdio.h> // Ensure this is included at the top of your file

void command_execution(t_data *pnt, t_tab_cmd *tab_cmd, int i, int *fd_pipe)
{
    printf("\033[32mExecuting command \033[35m#%d\033[0m: %s\033[0m\n", i, tab_cmd->cmd);
    printf("\033[32mArguments and options:\033[0m");
    for (int j = 0; tab_cmd->args[j] != NULL; j++) {
        printf("\033[37m[%s] \033[0m", tab_cmd->args[j]);
    }

    printf("\n\033[31mInput FD: %d, Output FD: %d\033[0m\n", tab_cmd->in_fd, tab_cmd->out_fd);
    printf("\033[31mPipe FDs: [R] = %d, [W] = %d\033[0m\n", fd_pipe[0], fd_pipe[1]);

    tab_cmd->pid = fork();
    if (tab_cmd->pid < 0)
        return ((void)error_out(pnt, "fork", 1));

    if (!tab_cmd->pid) // Child process
    {
        if (dup2(tab_cmd->in_fd, STDIN_FILENO) && tab_cmd->in_fd != -1)
            close(tab_cmd->in_fd);
        close(fd_pipe[0]);
        if (dup2(tab_cmd->out_fd, STDOUT_FILENO) && tab_cmd->out_fd != -1)
            close(tab_cmd->out_fd);
        set_mode(pnt, CHILD);

        dprintf(2, "\033[32mChild Process #%d: Executing %s\n\033[0m\n", getpid(), tab_cmd->cmd);
        execve(tab_cmd->cmd, tab_cmd->args, pnt->env);
        // If execve returns, it means there was an error
        error_out(pnt, tab_cmd->cmd, 1);
        total_clean(pnt);
        exit(1);
    }

    // Parent process
    close(fd_pipe[1]);
    if (pnt->fd_before != -1)
        close(pnt->fd_before);
    if (pnt->cmdt_count - 1 != i)
        pnt->fd_before = fd_pipe[0];
    else
        close(fd_pipe[0]);

    fd_cleaning(pnt, tab_cmd, i);
}


//function, redirects_cmd_tab, handles the redirections for a command in
//the t_tab_cmd structure based on the type of redirection specified.
//function, redirects_cmd_tab, handles the redirections for a command in
//the t_tab_cmd structure based on the type of redirection specified.

int	redirects_cmd_tab(t_data *pnt, t_tab_cmd *tab_cmd, int i)
{

	if (tab_cmd->redirections[i].type == REDIRECT_OUT)
	{
		if (tab_cmd->file_out != -1)
			close(tab_cmd->file_out);
		tab_cmd->file_out = open(tab_cmd->redirections[i].value,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (tab_cmd->file_out == -1)
			return (error_out(pnt, tab_cmd->redirections[i].value,1));
	}
	else if (tab_cmd->redirections[i].type == REDIRECT_APPEND)
	{
		if (tab_cmd->file_out != -1)
			close(tab_cmd->file_out);
		tab_cmd->file_out = open(tab_cmd->redirections[i].value,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (tab_cmd->file_out == -1)
			return (error_out(pnt, tab_cmd->redirections[i].value, 1));
	}
	else if (tab_cmd->redirections[i].type == REDIRECT_MULTILINE)
		if (create_heredoc(pnt, tab_cmd, i) == 1)
			return (1);
	return (0);
}

//This function, input_output_redirect, manages input and output
//redirection for a command specified in the t_tab_cmd structure

int	input_output_redirect(t_data *pnt, t_tab_cmd *tab_cmd)
{
	int	i;

	i = -1;
	while (tab_cmd->num_redirections > ++i)
	{
		if (tab_cmd->redirections[i].type != REDIRECT_MULTILINE
			&& tab_cmd->redirections[i].no_space == 2)
		{
			ft_printf_fd(2, "minishell: redirect to nowhere\n");
			pnt->code_exit = 1;
			return (1);
		}
		if (tab_cmd->redirections[i].type == REDIRECT_IN)
		{
			if (tab_cmd->file_in != -1)
				close(tab_cmd->file_in);
			tab_cmd->file_in = open(tab_cmd->redirections[i].value, O_RDONLY);
			if (tab_cmd->file_in == -1)
				return (error_out(pnt, tab_cmd->redirections[i].value, 1));
		}
		else if (redirects_cmd_tab(pnt, tab_cmd, i) == 1)
			return (1);
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

//the function is responsible for managing the execution of multiple
//commands in a shell program, handling pipelines, redirections, and
//executing both built-in and external commands.

void alt_exec_main(t_data *pnt)
{
    int i;
    int pip[2];

    i = -1;
    pnt->fd_before = -1;
    while (++i < pnt->cmdt_count)
    {
        // Print the command and its index
        // printf("\033[32mExecuting command #%d: %s\033[0m\n", i, pnt->cmdt[i].cmd);

        // Print each argument for the command. Since options are part of args, they will be printed here too.
        // printf("\033[32mArguments and options:\033[0m");
        // for (int j = 0; j < pnt->cmdt[i].num_args; j++)
        // {
        //     printf("\033[32m %s\033[0m", pnt->cmdt[i].args[j]);
        // }
        // Additional details can be printed as needed, such as file descriptors or redirection information
        // printf("\n\033[31mInput FD: %d, Output FD: %d\033[0m\n", pnt->cmdt[i].in_fd, pnt->cmdt[i].out_fd);

        if (pipe(pip) == -1)
            return ((void)error_out(pnt, "pipe", 1));

        if (input_output_redirect(pnt, &pnt->cmdt[i]) == 1 && pipelines_redirect(pnt, i, pip))
            continue;

        change_fd_input_output(pnt, &pnt->cmdt[i], pip, i);

        if (if_builtin(&pnt->cmdt[i]) == 1)
            shoot_builtin(pnt, &pnt->cmdt[i], i, pip);
        else
        {
            if (find_exec(pnt, &pnt->cmdt[i]) == 0
                && ++pnt->cmdt[i].is_child_process)
                command_execution(pnt, &pnt->cmdt[i], i, pip);
            else
                pipelines_redirect(pnt, i, pip);
        }
    }
    wait_for_childs(pnt);
}


