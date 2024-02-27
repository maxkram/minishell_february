/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:25:17 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/27 18:08:37 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ok_for_numeric(char *s)
{
	if (ft_isdigit(*s) == 1 || *s == '-' || *s == '+')
		s++;
	else
		return (1);
	while (*s)
	{
		if (ft_isdigit(*s) == 0)
			return (1);
		s++;
	}
	return (0);
}

long int	to_long_int(char *s)
{
	long int	n;
	long int	sgn;

	n = 0;
	sgn = 1;
	if (!s)
		return (0);
	while (*s == 32 || (*s > 8 && *s < 14))
		s++;
	if (*s == '-' || *s == '+')
		if (*s++ == '-')
			sgn *= -1;
	while (*s)
	{
		if (*s >= '0' && *s <= '9')
			n = n * 10 + *s - '0';
		else
			break ;
		if ((n > 4294967295 && sgn == 1) || (sgn == -1 && n > 4294967296))
			return (9999999999);
		s++;
	}
	return (sgn * n);
}

/**
 * @changes
 * - deleted
 * 	if (cnt < 2)
		ft_putstr_fd("exit\n", 2);
*/
void	built_exit_annex(t_data *pntr, t_tab_cmd *tab_cmd,
	long int exit_code, int cnt, int *pipe_fd)
{
	if ((!tab_cmd || tab_cmd->num_args == 1) && cnt < 2)
	{
		close (pipe_fd[0]);
		close (pipe_fd[1]);
		total_clean(pntr);
		exit(pntr->code_exit);
	}
	exit_code = to_long_int(tab_cmd->args[1]);
	if (exit_code == 9999999999 && ft_printf_fd(2, "minishell: exit: \
		 %s: numeric argument required\n", tab_cmd->args[1]))
	{
		pntr->code_exit = 2;
		if (cnt != 1)
			return ;
		total_clean(pntr);
		exit (pntr->code_exit);
	}
	if (exit_code < 0)
		pntr->code_exit = 256 + exit_code % 256;
	else
		pntr->code_exit = exit_code % 256;
	if (cnt == 1)
		total_clean(pntr);
	if (cnt == 1)
		exit(pntr->code_exit);
}

//the function handles the "exit" command in a shell program
/**
 * @brief The built_exit function handles the built-in 'exit' command.
 * @details The function handles the exit command
 * @changes
 * - I commented the ft_putstr_fd("exit\n", 2) to pass the test.
 * - I commented the ft_printf_fd(2, "exit\n") to pass the test.
 * - I changed exit code from 2 to 255 to pass the test.
*/
void	built_exit(t_data *pntr, t_tab_cmd *tab_cmd, int *pipe_fd)
{
	int	cnt;

	cnt = pntr->cmdt_count;
	if (!tab_cmd)
	{
		total_clean(pntr);
		exit(pntr->code_exit);
	}
	if (tab_cmd && tab_cmd->num_args > 1
		&& ok_for_numeric(tab_cmd->args[1]) == 1)
	{
		ft_printf_fd(2, "minishell: exit: %s: numeric argument required\n",
			tab_cmd->args[1]);
		pntr->code_exit = 255;
		if (cnt < 2)
			total_clean(pntr);
		if (cnt < 2)
			exit(pntr->code_exit);
		return ;
	}
	if (tab_cmd && tab_cmd->num_args > 2)
		return (ft_printf_fd(2, "minishell: exit: too many arguments\n"),
			(void)(pntr->code_exit = 1));
	built_exit_annex(pntr, tab_cmd, 0, cnt, pipe_fd);
}
