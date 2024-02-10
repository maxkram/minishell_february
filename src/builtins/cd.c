#include "minishell.h"

// Function to handle directory change
int change_dir(t_data *pnt, const char *path, const char *init_pwd) {
	// if (chdir(path) != 0) {
	//     perror("minishell: cd");
	//     return 1;
	// }
	return 0; // Return 0 on success, non-zero on failure
}

// Function to set environment variable
int set_variable(t_data *pnt, const char *var_name, const char *value) {
	// if (setenv(var_name, value, 1) != 0) {
	//     perror("minishell: setenv");
	//     return 1;
	// }
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
		print_error(pnt, "minishell: cd:", 1);
		return 1;
	}

	path = cmd_table->args[1];
	if (!path || path[0] == 0) {
		free(init_pwd);
		return 1;
	}

	if (change_dir(pnt, path, init_pwd) != 0 || pnt->cmdt_count != 1) {
		return 1;
	}

	free(init_pwd);
	temp = getcwd(NULL, 0);
	if (!temp) {
		print_error(pnt, "minishell: cd:", 1);
		return 1;
	}

	if (set_variable(pnt, "PWD", temp) != 0) {
		free(temp);
		return 1;
	}

	free(temp);
	return 0;
}

// Example usage
int main() {
	t_data pnt;
	t_tab_cmd cmd_table;

	// Initialize data and cmd_table as needed
	// ...

	// Call the builtin_cd function
	int result = builtin_cd(&pnt, &cmd_table);
	if (result == 0) {
		printf("Directory changed successfully!\n");
	} else {
		printf("Error changing directory.\n");
	}

	return 0;
}