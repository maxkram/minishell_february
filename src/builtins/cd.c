/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:23:43 by hezhukov          #+#    #+#             */
/*   Updated: 2024/03/01 18:15:22 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return (EXIT_SUCCESS);
}

int	change_folder(t_data *pnt, char *path, char *pwd)
{
	if (chdir(path) == -1)
	{
		ft_printf_fd(2, "minishell: cd: %s: ", path);
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
		return (EXIT_SUCCESS);
	}
	if (make_var(pnt, "OLDPWD", pwd) != 0 && ++pnt->code_exit)
		return (free(pwd), 1);
	return (EXIT_SUCCESS);
}

char	*get_target_path(t_tab_cmd *tab_cmd)
{
	char	*path;

	if (tab_cmd->num_args <= 1 || !tab_cmd->args[1]
		|| tab_cmd->args[1][0] == 0 || tab_cmd->args[1][0] == '~')
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (NULL);
		}
	}
	else
	{
		path = tab_cmd->args[1];
	}
	return (path);
}

int	change_directory_and_update_pwd(t_data *pnt, char *path)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd || change_folder(pnt, path, pwd) != 0 || pnt->cmdt_count != 1)
	{
		pnt->code_exit = 1;
		error_out(pnt, "", 1);
		return (1);
	}
	free(pwd);
	pwd = getcwd(NULL, 0);
	if (!pwd || make_var(pnt, "PWD", pwd) != 0)
	{
		free(pwd);
		pnt->code_exit = 1;
		return (1);
	}
	free(pwd);
	return (EXIT_SUCCESS);
}

int	built_cd(t_data *pnt, t_tab_cmd *tab_cmd)
{
	char	*path;

	pnt->code_exit = 0;
	path = get_target_path(tab_cmd);
	if (!path)
	{
		pnt->code_exit = 1;
		return (1);
	}
	return (change_directory_and_update_pwd(pnt, path));
}
