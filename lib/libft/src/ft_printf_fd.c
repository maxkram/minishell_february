/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 16:39:54 by ycyr-roy          #+#    #+#             */
/*   Updated: 2024/03/02 14:59:41 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbru(unsigned int n, int fd)
{
	char	c;
	int		i;

	i = count_dig(n);
	if (n >= 10)
	{
		ft_putnbr(n / 10, fd);
	}
	c = n % 10 + '0';
	write(fd, &c, 1);
	return (i);
}

int	ft_printhex(unsigned long n, char format, int fd)
{
	char	*base;
	int		ret;
	char	buffer[32];
	int		i;

	i = 0;
	ret = 0;
	base = "0123456789abcdef";
	if (format == 'p')
		ret += ft_putstr("0x", fd);
	if (format == 'X')
		base = "0123456789ABCDEF";
	if (n == 0)
		return (ret + ft_putchar('0', fd));
	while (n > 0)
	{
		buffer[i] = base[n % 16];
		n /= 16;
		i++;
	}
	while (i > 0)
		ret += ft_putchar(buffer[--i], fd);
	return (ret);
}

static int	ft_check(int fd, va_list args, const char format)
{
	unsigned long	ptr;

	if (format == 'c')
		return (ft_putchar(va_arg(args, int), fd));
	if (format == '%')
		return (ft_putchar('%', fd));
	if (format == 's')
		return (ft_putstr(va_arg(args, char *), fd));
	if (format == 'd' || format == 'i')
		return (ft_putnbr(va_arg(args, int), fd));
	if (format == 'u')
		return (ft_putnbru(va_arg(args, int), fd));
	else if (format == 'x' || format == 'X')
		return (ft_printhex(va_arg(args, unsigned int), format, fd));
	if (format == 'p')
	{
		ptr = va_arg(args, unsigned long);
		if (!ptr)
			return (ft_putstr("(nil)", fd));
		return (ft_printhex(ptr, format, fd));
	}
	return (0);
}

int	ft_printf_fd(int fd, const char *format, ...)
{
	va_list	args;
	int		i;
	int		ret;

	va_start(args, format);
	i = 0;
	ret = 0;
	while (format[i])
	{
		if (format[i] == '%' && ft_strchr("cspdiuxX%", format[i + 1]) != 0)
		{
			ret += ft_check(fd, args, format[i + 1]);
			i++;
		}
		else
			ret += ft_putchar(format[i], fd);
		i++;
	}
	va_end(args);
	return (ret);
}
