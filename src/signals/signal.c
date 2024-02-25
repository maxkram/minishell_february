/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:27:41 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/25 15:54:21 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_global_signal = 0;

void	sigint_manager(int status)
{
	if (status == SIGINT)
	{
		g_global_signal = 1;
		ft_putchar_fd('\n', STDERR_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	manage_multiline(int status)
{
	if (status == SIGINT)
	{
		g_global_signal = 1;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

void	start_signals(t_data *pnt)
{
	if (pnt->mode == INTERACT)
	{
		signal(SIGINT, &sigint_manager);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (pnt->mode == NON_INTERACT)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (pnt->mode == CHILD)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else if (pnt->mode == MULTILINE)
	{
		signal(SIGINT, &manage_multiline);
		signal(SIGQUIT, SIG_IGN);
	}
}

void	set_mode(t_data *pnt, t_set_mode mode)
{
	pnt->mode = mode;
	start_signals(pnt);
}
