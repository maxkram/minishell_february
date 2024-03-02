/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:27:41 by hezhukov          #+#    #+#             */
/*   Updated: 2024/03/02 12:30:15 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>

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

void	ft_setup_term(void)
{
	struct termios	t;

	tcgetattr(0, &t);
	t.c_lflag &= ~ECHOCTL;
	tcsetattr(0, TCSANOW, &t);
}

void	start_signals(t_data *pnt)
{
	if (pnt->mode == INTERACT)
	{
		ft_setup_term();
		signal(SIGINT, &sigint_manager);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (pnt->mode == NON_INTERACT)
	{
		ft_setup_term();
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
		ft_setup_term();
		signal(SIGINT, &manage_multiline);
		signal(SIGQUIT, SIG_IGN);
	}
}

void	set_mode(t_data *pnt, t_set_mode mode)
{
	pnt->mode = mode;
	start_signals(pnt);
}
