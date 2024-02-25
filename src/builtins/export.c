/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:26:14 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/25 16:57:41 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static char	**realloc_env_vars(t_data *data, int size)
// {
// 	char	**new_env;
// 	int		i;

// 	new_env = ft_calloc(size + 1, sizeof * new_env);
// 	if (!new_env)
// 		return (NULL);
// 	i = 0;
// 	while (data->env[i] && i < size)
// 	{
// 		new_env[i] = ft_strdup(data->env[i]);
// 		free_ptr(data->env[i]);
// 		i++;
// 	}
// 	free(data->env);
// 	return (new_env);
// }

// int	get_env_var_index(char **env, char *var)
// {
// 	int		i;
// 	char	*tmp;

// 	tmp = ft_strjoin(var, "=");
// 	if (!tmp)
// 		return (-1);
// 	i = 0;
// 	while (env[i])
// 	{
// 		if (ft_strncmp(tmp, env[i], ft_strlen(tmp)) == 0)
// 		{
// 			free_ptr(tmp);
// 			return (i);
// 		}
// 		i++;
// 	}
// 	free_ptr(tmp);
// 	return (-1);
// }

// bool	set_env_var(t_data *data, char *key, char *value)
// {
// 	int		idx;
// 	char	*tmp;

// 	idx = get_env_var_index(data->env, key);
// 	if (value == NULL)
// 		value = "";
// 	tmp = ft_strjoin("=", value);
// 	if (!tmp)
// 		return (false);
// 	if (idx != -1 && data->env[idx])
// 	{
// 		free_ptr(data->env[idx]);
// 		data->env[idx] = ft_strjoin(key, tmp);
// 	}
// 	else
// 	{
// 		idx = env_var_count(data->env);
// 		data->env = realloc_env_vars(data, idx + 1);
// 		if (!data->env)
// 			return (false);
// 		data->env[idx] = ft_strjoin(key, tmp);
// 	}
// 	free_ptr(tmp);
// 	return (true);
// }

// bool	is_valid_env_var_key(char *var)
// {
// 	int	i;

// 	i = 0;
// 	if (ft_isalpha(var[i]) == 0 && var[i] != '_')
// 		return (false);
// 	i++;
// 	while (var[i] && var[i] != '=')
// 	{
// 		if (ft_isalnum(var[i]) == 0 && var[i] != '_')
// 			return (false);
// 		i++;
// 	}
// 	return (true);
// }

// int	built_export(t_data *data, t_tab_cmd *cmd)
// {
// 	int		i;
// 	char	*key;
// 	char	*value;
// 	char	*equal_sign_pos;

// 	if (cmd->args[1] == NULL)
// 		print_env_vars(data);
// 	i = 1;
// 	while (cmd->args[i])
// 	{
// 		if (!is_valid_env_var_key(cmd->args[i]))
// 			error_message(" not a valid identifier", EXIT_FAILURE);
// 		else if (ft_strchr(cmd->args[i], '='))
// 		{
// 			equal_sign_pos = ft_strchr(cmd->args[i], '=');
// 			key = ft_strndup(cmd->args[i], equal_sign_pos - cmd->args[i]);
// 			value = ft_strdup(equal_sign_pos + 1);
// 			set_env_var(data, key, value);
// 			free(key);
// 			free(value);
// 		}
// 		i++;
// 	}
// 	return (EXIT_SUCCESS);
// }


int	has_no_has(const char *set, char c)
{
	if (!set)
		return (0);
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

int	probel(const char *str)
{
	const char	arr[] = " \v\f\r\n\t";

	if (!str)
		return (1);
	while (has_no_has(arr, *str))
		str++;
	return (!*str);
}

static int	validity(char *str)
{
	if (str == NULL || !(ft_isalpha(*str) || *str == '_'))
		return (1);
	while (*str && *str != '=')
	{
		if (!(ft_isalnum(*str) || *str == '_'))
			return (1);
		str++;
	}
	return (0);
}

static void	check_norm(t_data *data, char *line)
{
	char	*value;
	char	*key;
	int		i;

	if (!line || !*line || !has_no_has(line, '='))
		return ;
	i = 0;
	while (line[i] && line[i] != '=')
		i++;
	key = ft_substr(line, 0, i++);
	if (!key)
		return ;
	while (line[i])
		i++;
	value = ft_substr(line, ft_strlen(key) + 1, i);
	if (!value)
	{
		free(key);
		return ;
	}
	make_var(data, key, value);
	free(value);
	free(key);
}

static void	no_args(char **envp)
{
	int	vvod;
	int	i;

	vvod = 0;
	while (envp[vvod])
	{
		i = 0;
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		while (envp[vvod][i] && envp[vvod][i] != '=')
			ft_putchar_fd(envp[vvod][i++], STDOUT_FILENO);
		if (envp[vvod][i] == '=')
		{
			ft_putchar_fd(envp[vvod][i++], STDOUT_FILENO);
			ft_putchar_fd('"', STDOUT_FILENO);
			while (envp[vvod][i])
				ft_putchar_fd(envp[vvod][i++], STDOUT_FILENO);
			ft_putchar_fd('"', STDOUT_FILENO);
		}
		write(STDOUT_FILENO, "\n", 1);
		vvod++;
	}
}

void	built_export(t_data *data, t_tab_cmd *cmd_table)
{
	int		i;

	data->code_exit = 0;
	if (cmd_table->num_args == 1)
		no_args(data->env);
	i = 1;
	while (i < cmd_table->num_args)
	{
		if (!is_valid_env_var_key(cmd->args[i]))
			set_error_and_code(cmd->args[i], &data->code_exit);
		else if (ft_strchr(cmd->args[i], '='))
		{
			equal_sign_pos = ft_strchr(cmd->args[i], '=');
			key = ft_strndup(cmd->args[i], equal_sign_pos - cmd->args[i]);
			value = ft_strdup(equal_sign_pos + 1);
			set_env_var(data, key, value);
			free(key);
			free(value);
		}
		i++;
	}
}
