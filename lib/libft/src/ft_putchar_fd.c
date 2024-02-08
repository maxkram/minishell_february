/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 17:55:04 by hezhukov          #+#    #+#             */
/*   Updated: 2023/11/01 16:41:04 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
function: writes a character to a file descriptor (fd)
   input: ft_putchar_fd('a', 1)
  output: a
*/

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
