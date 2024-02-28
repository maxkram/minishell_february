/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:21 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/22 18:28:24 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_string_array(char **pnt)
{
	int	i;

	i = 0;
	if (pnt)
	{
		while (pnt[i])
		{
			free(pnt[i]);
			pnt[i] = NULL;
			i++;
		}
		free(pnt);
		pnt = NULL;
	}
}

void	fd_exit(t_data *pntr, int code_exit)
{
	if (code_exit == 1)
		perror("An error!");
	total_clean(pntr);
	exit(1);
}

int	ft_putstr_int(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s)
		return (EXIT_SUCCESS);
	while (*s)
	{
		ft_putchar_fd(*s++, fd);
		i++;
	}
	return (i);
}
