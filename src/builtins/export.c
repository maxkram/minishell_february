/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:26:14 by hezhukov          #+#    #+#             */
/*   Updated: 2024/03/02 12:37:59 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* realloc_env_vars: Reallocate the environment variable array to a new size.
* This function creates a new array of environment variables with the specified
* size, copies the existing variables into the new array, frees the old array,
* and returns the new array. The new array is initially filled with NULLs,
* ensuring that any unused slots are properly null-terminated.
*
* @param pnt: Pointer to the data structure containing the env variables array.
* @param size: The new size for the environment variables array.
* @return char**: Returns the newly allocated environment variables array.
*/
static char	**realloc_env_vars(t_data *pnt, int size)
{
	char	**new_env;
	int		i;

	new_env = ft_calloc(size + 1, sizeof * new_env);
	if (!new_env)
		return (NULL);
	i = 0;
	while (pnt->env[i] && i < size)
	{
		new_env[i] = ft_strdup(pnt->env[i]);
		free_ptr(pnt->env[i]);
		i++;
	}
	free(pnt->env);
	return (new_env);
}

/**
** get_env_var_index: Find the index of a specific environment variable.
** This function searches for a given environment variable by its name within
** the current list of environment variables. It concatenates the variable name
** with an = sign to match the format used in environment variables (KEY=value).
** If the variable is found, it returns its index; otherwise, it returns -1.
**
** @param env: The array of environment variables.
** @param var: The name of the environment variable to find.
** @return int: The index of the environment variable if found, -1 otherwise.
*/
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

/**
 * set_env_var: Set or update an environment variable.
 * This function sets a new environment variable or updates the value of an
 * existing one. If the variable already exists, its value is updated; if not,
 * the variable is added to the environment. The environment variables array
 * may be reallocated if necessary to accommodate the new variable.
 * @param pnt: Pointer to the data structure containing the env variables array.
 * @param key: The name of the environment variable to set or update.
 * @param value: The value to set for the environment variable.
 * @param index:
 * @return bool: Returns true if the operation is successful, false otherwise.
*/
bool	set_env_var(t_data *pnt, char *key, char *value)
{
	int		index;
	char	*new_value;

	index = get_env_var_index(pnt->env, key);
	if (value == NULL)
		value = "";
	new_value = ft_strjoin("=", value);
	if (!new_value)
		return (FALSE);
	if (index != -1 && pnt->env[index])
	{
		free_ptr(pnt->env[index]);
		pnt->env[index] = ft_strjoin(key, new_value);
	}
	else
	{
		index = env_var_count(pnt->env);
		pnt->env = realloc_env_vars(pnt, index + 1);
		if (!pnt->env)
			return (false);
		pnt->env[index] = ft_strjoin(key, new_value);
	}
	free_ptr(new_value);
	return (TRUE);
}

/**
 * is_valid_env_var_key: Validates environment variable names.
 * Checks if a string can be a valid environment variable name, starting with an
 * alphabet character or underscore, followed by alphanumeric characters or
 * underscores.
 * @param var: The string to validate.
 * @return bool: Returns true if valid, false otherwise.
 *
 * Valid names:
 * - PATH: All uppercase, common convention.
 * - USER_ID: Uppercase with underscores, clear and readable.
 * - _API_KEY: Starts with underscore, then uppercase letters.
 * - dbPort: Mixed case; valid, though uppercase is common.
 * - config_file_path: Lowercase with underscores; valid, less common.
 *
 * Invalid names:
 * - 9PATH: Starts with a digit.
 * - USER ID: Contains a space
 * - API-KEY: Contains a dash.
 * - *SECRET*: Not allowed special characters (*).
 * - .DOTFILE: Starts with a dot.
 */
bool	is_valid_env_var_key(char *var)
{
	int	i;

	i = 0;
	if (ft_isalpha(var[i]) == FALSE && var[i] != '_')
		return (FALSE);
	i++;
	while (var[i] && var[i] != '=')
	{
		if (ft_isalnum(var[i]) == FALSE && var[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

/**
 * built_export: 'export' command for setting environment variables.
 * Executes the 'export' shell command functionality.
 * Without arguments, it lists  * all environment variables.
 * With arguments, it sets variables in 'KEY=value' format.
 * Invalid variable names trigger an error.
 *
 * @param pnt: Contains the environment variables array.
 * @param cmd: Contains the 'export' command arguments.
 * @return int: EXIT_SUCCESS on successful execution.
 */
int	built_export(t_data *pnt, t_tab_cmd *cmd)
{
	int		i;
	char	*key;
	char	*value;
	char	*equal_sign_pos;

	if (cmd->args[1] == NULL)
		print_env_vars(pnt);
	i = 1;
	while (cmd->args[i])
	{
		if (!is_valid_env_var_key(cmd->args[i]))
			set_error_and_code(cmd->args[i], &pnt->code_exit);
		else if (ft_strchr(cmd->args[i], '=') && pnt->cmdt_count == 1)
		{
			equal_sign_pos = ft_strchr(cmd->args[i], '=');
			key = ft_strndup(cmd->args[i], equal_sign_pos - cmd->args[i]);
			value = ft_strdup(equal_sign_pos + 1);
			set_env_var(pnt, key, value);
			free(key);
			free(value);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
