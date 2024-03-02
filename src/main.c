/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:25:05 by hezhukov          #+#    #+#             */
/*   Updated: 2024/03/02 14:03:05 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[], char **env_p)
{
	t_data	pnt;

	(void)argv;
	if (argc != 1)
		return (ft_putstr("Error: too many arguments\n", 2));
	init_pnt(&pnt, env_p);
	while (1)
	{
		set_mode(&pnt, INTERACT);
		pnt.input = readline(PROMPT);
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
