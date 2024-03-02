/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 16:39:54 by ycyr-roy          #+#    #+#             */
/*   Updated: 2024/03/02 14:58:21 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putchar(char c, int fd)
{
	write(fd, &c, 1);
	return (1);
}

int	ft_putstr(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (*s)
	{
		ft_putchar(*s++, fd);
		i++;
	}
	return (i);
}

int	count_dig(int n)
{
	int	i;

	i = 1;
	if (n == -2147483648)
		return (11);
	if (n < 0)
	{
		i++;
		n = -n;
	}
	while (n > 9)
	{
		i++;
		n = n / 10;
	}
	return (i);
}

int	ft_putnbr(int n, int fd)
{
	char	c;
	int		i;

	i = count_dig(n);
	if (n < 0)
	{
		if (n == -2147483648)
		{
			ft_putnbr(n / 10, fd);
			write(fd, "8", 1);
			return (i);
		}
		write(fd, &"-", 1);
		n = -n;
	}
	if (n >= 10)
	{
		ft_putnbr(n / 10, fd);
	}
	c = n % 10 + '0';
	write(fd, &c, 1);
	return (i);
}
