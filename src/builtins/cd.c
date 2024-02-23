/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:23:43 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/22 18:30:48 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Function to set environment variable
int	make_var(t_data *pnt, char *var_name, char *value)
{
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
	return (0);
}

/**
 * @changes
 * - deleted change_dir_aux(pnt, path, pwd);
 * - deleted change_dir_aux(pnt, pwd, pwd);
*/
int	change_folder(t_data *pnt, char *path, char *pwd)
{
	if (chdir(path) == -1)
	{
		ft_printf_fd(2, "minishell: cd: %s: ", path);
		perror("");
		pnt->code_exit = 1;
		free(pwd);
		return (1);
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
		}
		free(pwd);
		return (0);
	}
	if (make_var(pnt, "OLDPWD", pwd) != 0 && ++pnt->code_exit)
		return (free(pwd), 1);
	return (0);
}

/**
 * @brief The built_cd function handles the built-in 'cd' command.
 * @changes
 * - I changed the return value if the number of arguments is greater than 2.
 * 		- I changed it from 1 to 0 to pass the test.
 * - deleted  return (0);
*/
int	built_cd(t_data *pnt, t_tab_cmd *tab_cmd)
{
	char	*path;
	char	*tmp;
	char	*pwd;

	pnt->code_exit = 0;
	if (tab_cmd->num_args > 2)
		return (pnt->code_exit = 0);
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (pnt->code_exit = 1, error_out(pnt, "minishell: cd:", 1));
	path = tab_cmd->args[1];
	if (!path || path[0] == 0)
		return (free(pwd), 1);
	if (change_folder(pnt, path, pwd) != 0 || pnt->cmdt_count != 1)
		return (1);
	free(pwd);
	tmp = getcwd(NULL, 0);
	if (!tmp)
		return (pnt->code_exit = 1, error_out(pnt, "minishell: cd:", 1));
	if (make_var(pnt, "PWD", tmp) != 0)
		return (pnt->code_exit = 1, free(tmp), 1);
	return (free(tmp), 0);
}
