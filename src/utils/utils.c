/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:43 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/28 19:58:30 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data	*get_data(void)
{
	static t_data	data;

	return (&data);
}

void	ft_error(char *str)
{
	ft_putstr_fd(ERR_MAIN, STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	perror(ERR_EXP);
	exit(ERROR);
}

int	error_out(t_data *pnt, char *s, int n)
{
	if (n == 1)
	{
		ft_printf_fd(2, "minishell: %s: ", s);
		perror("");
		pnt->code_exit = n;
	}
	return (1);
}

void	safe_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	error_in_syntax(int type, t_data *pnt)
{
	pnt->code_exit = 2;
	ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
	if (type == 39)
		ft_putstr_fd("`''\n", 2);
	else if (type == 0)
		ft_putstr_fd("`newline'\n", 2);
	else if (type == 34)
		ft_putstr_fd("`\"'\n", 2);
	else if (type == 5)
		ft_putstr_fd("`<<'\n", 2);
	else if (type == 4)
		ft_putstr_fd("`>>'\n", 2);
	else if (type == 3)
		ft_putstr_fd("`>'\n", 2);
	else if (type == 2)
		ft_putstr_fd("`<'\n", 2);
	else if (type == 1)
		ft_putstr_fd("`|'\n", 2);
}
