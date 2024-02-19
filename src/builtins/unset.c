#include "minishell.h"

// static int	unset_valid(char *str)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i])
// 	{
// 		if (ft_isalnum(str[i]) == FALSE && str[i] != '_')
// 			return (FALSE);
// 		i++;
// 	}
// 	return (TRUE);
// }

// static void	env_removeline(t_data *data, char *str)
// {
// 	int		i;
// 	int		j;
// 	char	**new_env;

// 	i = 0;
// 	j = 0;
// 	while (data->env[i])
// 		i++;
// 	new_env = ft_calloc(i, sizeof(char *));
// 	i = 0;
// 	while (data->env[i])
// 	{
// 		if (ft_strncmp(str, data->env[i], ft_strlen(str)) != 0)
// 		{
// 			new_env[j] = data->env[i];
// 			j++;
// 		}
// 		else
// 		{
// 			// free(data->env[i]); // not working, crashing... why?
// 		}
// 		i++;
// 	}
// 	free(data->env);
// 	data->env = new_env;
// }

// void	built_unset(char **args, int argc, t_data *data)
// {
// 	int	i;

// 	i = 1;
// 	while(i < argc)
// 	{
// 		if (unset_valid(args[i]) == FALSE)
// 			my_error(ERR_UNSET);
// 		else if (var_in_env(args[i]) == TRUE)
// 			env_removeline(data, args[i]);
// 		else
// 			printf("not in env\n");
// 		i++;
// 	}
// }

int	array_length(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

int	change_place(int flag, int i, int j)
{
	if (flag)
		return (i);
	return (j);
}

// int remove_vars_of_arr(t_data *pnt, char *key)
// {
// 	char	**arr;
// 	// int		place;
// 	const int	place = variable_index(pnt->env, key);
// 	int		size;
// 	int		i;

// 	// place = variable_index(pnt->env, key);
// 	if (!place)
// 		return (1);
// 	size = array_length(pnt->env);
// 	arr = (char **)ft_calloc(sizeof(char *), size);
// 	if (!arr)
// 		return (2);
// 	i = 0;
// 	while (size > i)
// 	{
// 		arr[i] = pnt->env[change_place(place > i, i, i + 1)];
// 		i++;
// 	}
// 	free(pnt->env[place]);
// 	free(pnt->env);
// 	pnt->env = arr;
// 	return (0);
// }

int	remove_vars_of_arr(t_data *pnt, char *peg)
{
	char		**arr;
	int			place;
	int			size;
	int			i;

	place = variable_index(pnt->env, peg);
	i = 0;
	if (place == -1)
		return (1);
	size = array_length(pnt->env);
	arr = (char **)ft_calloc(sizeof (char *), size);
	if (!arr)
		return (2);
	while (i < size)
	{
		arr[i] = pnt->env[change_place(i < place, i, i + 1)];
		i++;
	}
	free(pnt->env[place]);
	free(pnt->env);
	pnt->env = arr;
	return (0);
}

void	built_unset(t_data *pnt, t_tab_cmd *tab_cmd)
{
	int	i;

	i = 1;
	pnt->code_exit = 0;
	if (pnt->cmdt_count == 1)
	{
		while (tab_cmd->num_args > i)
		{
			if (remove_vars_of_arr(pnt, tab_cmd->args[i]) == 2)
			{
				error_out(pnt, "Memory allocation error", 1);
				return ;
			}
			i++;
		}
	}
	else
		return ;
}
