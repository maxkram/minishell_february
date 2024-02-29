/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:25:05 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/29 11:36:40 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**path_getter(t_data *pnt, int i)
{
	char	**result;
	char	*found;

	while (pnt->env[++i])
	{
		// found = ft_strstr(pnt->env[i], "PATH=");
		found = ft_strstr(pnt->env[i], "PATH=/home");
		if (found != NULL)
			break ;
	}
	if (pnt->env[i] == NULL)
		return (NULL);
	result = ft_split_fd(found, ':');
	if (!result)
	{
		free(result);
		return (error_out(pnt, "ft_split", 1), NULL);
	}
	return (result);
}

void	env_init(t_data *pnt, char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	pnt->env = ft_calloc(i + 1, sizeof(char *));
	i = 0;
	while (env[i])
	{
		pnt->env[i] = ft_strdup_fd(env[i]);
		i++;
	}
}

int	main(int argc, char *argv[], char **env_p)
{
	t_data	pnt;

	(void)argv;
	(void)argc;
	if (argc != 1)
		return (ft_putstr("Error: too many arguments\n", 2));
	init_pntr(&pnt, env_p);
	while (1)
	{
		set_mode(&pnt, INTERACT);
		pnt.input = readline("\033[31m\033[1mminishell$ \033[0m");
		set_mode(&pnt, NON_INTERACT);
		if (g_global_signal == 1 && g_global_signal--)
			pnt.code_exit = 130;
		if (!pnt.input)
			built_exit(&pnt, NULL);
		if (pnt.input[0] != '\0')
			add_history(pnt.input);
		pnt.path = path_getter(&pnt, -1);
		if (tokener(&pnt) == 0 && extender(&pnt) == 0 && parser(&pnt) == 0)
			execution(&pnt);
		if (pnt.fd_before != -1)
			close(pnt.fd_before);
		cmdt_cleaning(&pnt);
		pntr_cleaning(&pnt);
	}
}
