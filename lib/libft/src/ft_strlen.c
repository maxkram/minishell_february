/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:53:13 by hezhukov          #+#    #+#             */
/*   Updated: 2023/10/30 17:51:16 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
function: calculates the length of the string (s), excluding the terminating
		  null byte ('\0').
   input: ft_strlen("Hello")
  output: 5
*/

size_t	ft_strlen(const char *s)
{
	const char	*a;

	if (!s)
		return (0);
	a = s;
	while (*s)
		s++;
	return (s - a);
}
