#include "minishell.h"

// The function creates a child process to execute the specified command,
//handling input and output redirection, executes the commands using
//execve and managing file descriptors in the parent process.

void	command_execution(t_data *pnt, t_tab_cmd *tab_cmd, int i, int *fd_pipe)
{
	tab_cmd->pid = fork();
	if (tab_cmd->pid < 0)
		return ((void)error_out(pnt, "fork", 1));
	if (!tab_cmd->pid)
	{
		if (dup2(tab_cmd->in_fd, STDIN_FILENO) && tab_cmd->in_fd != -1)
			close(tab_cmd->in_fd);
		close(fd_pipe[0]);
		if (dup2(tab_cmd->out_fd, STDOUT_FILENO) && tab_cmd->out_fd != -1)
			close(tab_cmd->out_fd);
		set_mode(pnt, CHILD);
		// dprintf(2, "cmd: %s\n", tab_cmd->cmd);
		// dprintf(2, "args0: %s\n", tab_cmd->args[0]);
		// dprintf(2, "args1: %s\n", tab_cmd->args[1]);
		// dprintf(2, "args2: %s\n", tab_cmd->args[2]);
		execve(tab_cmd->cmd, tab_cmd->args, pnt->env);
		error_out(pnt, tab_cmd->cmd, 1);
		total_clean(pnt);
		exit(1);
	}
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

void	alt_exec_main(t_data *pnt)
{
	int	i;
	int	pip[2];

	i = -1;
	pnt->fd_before = -1;
	while (++i < pnt->cmdt_count)
	{
		if (pipe(pip) == -1)
			return ((void)error_out(pnt, "pipe", 1));
		if (input_output_redirect(pnt, &pnt->cmdt[i]) == 1 && pipelines_redirect(pnt, i, pip))
			continue ;
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

/* 				NEW EXECUTION 									*/

void	create_pipes(int pipefds[], int n_pipes)
{
	int	i;

	i = 0;
	while (i < n_pipes)
	{
		if (pipe(pipefds + i * 2) < 0)
			// error_message("pipe", 1); // @todo add this function to minishell project
		i++;
	}
}

void	create_child_process(t_pipex_data *pipeline, int cmd_index)
{
	int	j;

	if (cmd_index == 0 && pipeline->here_doc == true)
	{
		(void)cmd_index;
		// redirect_here_doc(pipeline);
	}
	else if (cmd_index == 0)
		redirect_first_command(pipeline);
	if (cmd_index == pipeline->n_cmds - 1)
		redirect_last_command(pipeline);
	if (cmd_index > 0)
	{
		close(pipeline->pipefds[(cmd_index - 1) * 2 + 1]);
		if (dup2(pipeline->pipefds[(cmd_index - 1) * 2], STDIN_FILENO) < 0)
			error_message("dup2 (stdin)", 1);
	}
	if (cmd_index < pipeline->n_cmds - 1)
	{
		close(pipeline->pipefds[cmd_index * 2]);
		if (dup2(pipeline->pipefds[cmd_index * 2 + 1], STDOUT_FILENO) < 0)
			error_message("dup2 (stdout)", 1);
	}
	j = 0;
	while (j < 2 * pipeline->n_pipes)
		close(pipeline->pipefds[j++]);
	execute_command(pipeline->argv[cmd_index], pipeline);
	error_message("execute_command", 1);
}

void	execute_pipeline(t_data *pnt)
{
	int		i;
	pid_t	pid;

	create_pipes(pnt->pipex_data.pipefds, pnt->pipex_data.n_pipes); // how to check if this function is working properly
	i = 0;
	while (i < pnt->pipex_data.n_cmds)
	{
		pid = fork();
		if (pid == 0)
			create_child_process(&pnt->pipex_data, i);
		else if (pid < 0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else
			close_unused_pipe_ends(&pnt->pipex_data, i);
		i++;
	}
	close_all_pipe_fds(&pnt->pipex_data);
	cleanup_pipes_and_wait(&pnt->pipex_data);
}

