#include "minishell.h"

void	free_ptr(void *ptr)
{
	if (ptr != NULL)
	{
		free(ptr);
		ptr = NULL;
	}
}

static char	**realloc_env_vars(t_data *data, int size)
{
	char	**new_env;
	int		i;

	new_env = ft_calloc(size + 1, sizeof * new_env);
	if (!new_env)
		return (NULL);
	i = 0;
	while (data->env[i] && i < size)
	{
		new_env[i] = ft_strdup(data->env[i]);
		free_ptr(data->env[i]);
		i++;
	}
	free(data->env);
	return (new_env);
}

int	env_var_count(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

void	free_str_tab(char **tab)
{
	int	i;

	i = 0;
	if (tab)
	{
		while (tab[i])
		{
			if (tab[i])
			{
				free_ptr(tab[i]);
				tab[i] = NULL;
			}
			i++;
		}
		free(tab);
		tab = NULL;
	}
}

int	get_env_var_index(char **env, char *var)
{
	int		i;
	char	*tmp;

	tmp = ft_strjoin(var, "=");
	if (!tmp)
		return (-1);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(tmp, env[i], ft_strlen(tmp)) == 0)
		{
			free_ptr(tmp);
			return (i);
		}
		i++;
	}
	free_ptr(tmp);
	return (-1);
}

bool	set_env_var(t_data *data, char *key, char *value)
{
	int		idx;
	char	*tmp;

	idx = get_env_var_index(data->env, key);
	if (value == NULL)
		value = "";
	tmp = ft_strjoin("=", value);
	if (!tmp)
		return (false);
	if (idx != -1 && data->env[idx])
	{
		free_ptr(data->env[idx]);
		data->env[idx] = ft_strjoin(key, tmp);
	}
	else
	{
		idx = env_var_count(data->env);
		data->env = realloc_env_vars(data, idx + 1);
		if (!data->env)
			return (false);
		data->env[idx] = ft_strjoin(key, tmp);
	}
	free_ptr(tmp);
	return (true);
}

bool	is_valid_env_var_key(char *var)
{
	int	i;

	i = 0;
	if (ft_isalpha(var[i]) == 0 && var[i] != '_')
		return (false);
	i++;
	while (var[i] && var[i] != '=')
	{
		if (ft_isalnum(var[i]) == 0 && var[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

/**
 * @brief built_export function is used to set the environment variables
 * @details
 * 1. The built_export function is used to set the environment variables.
 * 2. The function iterates through the command arguments and sets the environment variables.
 * 3. If the argument is a valid environment variable key, the function sets the environment variable.
 * 4. If the argument contains a key-value pair, the function splits the key-value pair and sets the environment variable.
 * 5. The function returns the status of the command execution.
 * @param data The structure containing the shell data
 * @param cmd The structure containing the command data
 * @return int The status of the command execution
 * @retval 0 If the command is executed successfully
 * @retval 1 If the command execution fails
*/

int	built_export(t_data *data, t_tab_cmd *cmd)
{
	int		i;
	int		ret;
	char	*key;
	char	*value;

	ret = EXIT_SUCCESS;
	i = 1;
	while (cmd->args[i])
	{
		if (!is_valid_env_var_key(cmd->args[i]))
			error_message(" not a valid identifier", EXIT_FAILURE);
		else if (ft_strchr(cmd->args[i], '='))
		{
			char *equal_sign_pos = ft_strchr(cmd->args[i], '=');
			key = ft_strndup(cmd->args[i], equal_sign_pos - cmd->args[i]);
			value = ft_strdup(equal_sign_pos + 1);
			set_env_var(data, key, value);
			free(key);
			free(value);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

