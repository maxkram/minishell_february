#include "minishell.h"

// Function to set environment variable
int make_var(t_data *pnt, const char *var_name, const char *value)
{
	// if (setenv(var_name, value, 1) != 0) {
	//     perror("minishell: setenv");
	//     return 1;
	// }
	char	*tmp;
	char	*nvar;
	int		i;

	i = variable_index(pnt->env, var_name);
	if (i == -1)
		return (create_env_var(pnt, var_name, value));
	tmp = ft_strcat(var_name, "=");
	if (!tmp)
		return (2);
	nvar = ft_strcat(tmp, value);
	free(tmp);
	free(pnt->env[i]);
	pnt->env[i] = nvar;
	return (0); // Return 0 on success, non-zero on failure
}

// int	change_dir_aux(t_data *pnt, char *path, char *pwd)
// {
// 	ft_printf_fd(2, "minishell: cd: %s: ", path);
// 	perror("");
// 	pnt->code_exit = 1;
// 	free(pwd);
// 	return (1);
// }

// Function to handle directory change
int change_folder(t_data *pnt, const char *path, const char *pwd) {
	// if (chdir(path) != 0) {
	//     perror("minishell: cd");
	//     return 1;
	// }
	if (chdir(path) == -1)
	{
		ft_printf_fd(2, "minishell: cd: %s: ", path);
		perror("");
		pnt->code_exit = 1;
		free(pwd);
		return (1);
		change_dir_aux(pnt, path, pwd);
	}
	if (pnt->cmdt_count != 1)
	{
		if (chdir(pwd) == -1)
		{
			ft_printf_fd(2, "minishell: cd: %s: ", pwd);
			perror("");
			pnt->code_exit = 1;
			free(pwd);
			return (1);
			// change_dir_aux(pnt, pwd, pwd);

		}
		free(pwd);
		return (0);
	}
	if (make_var(pnt, "OLDPWD", pwd) != 0 && ++pnt->code_exit)
		return (free(pwd) , 1);
	return 0; // Return 0 on success, non-zero on failure
}


// Function to handle built-in 'cd' command
int builtin_cd(t_data *pnt, t_tab_cmd *cmd_table) {
	char *path;
	char *temp;
	char *init_pwd;

	pnt->code_exit = 0;
	if (cmd_table->num_args > 2) {
		ft_printf_fd(2, "minishell: cd: too many arguments\n");
		return 1;
	}

	init_pwd = getcwd(NULL, 0);
	if (!init_pwd) {
		error_out(pnt, "minishell: cd:", 1);
		return 1;
	}

	path = cmd_table->args[1];
	if (!path || path[0] == 0) {
		free(init_pwd);
		return 1;
	}

	if (change_folder(pnt, path, init_pwd) != 0 || pnt->cmdt_count != 1) {
		return 1;
	}

	free(init_pwd);
	temp = getcwd(NULL, 0);
	if (!temp) {
		error_out(pnt, "minishell: cd:", 1);
		return 1;
	}

	if (make_var(pnt, "PWD", temp) != 0) {
		free(temp);
		return 1;
	}

	free(temp);
	return 0;
}
