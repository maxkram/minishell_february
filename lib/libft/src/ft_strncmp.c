/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 20:24:00 by hezhukov          #+#    #+#             */
/*   Updated: 2023/11/03 13:59:53 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
function: compares not more than (n) characters of the string (str1) to those
		  of the string (str2).
   input: ft_strncmp("abcdef", "abc", 6)
  output: 0

   input: ft_strncmp("abcdef", "abcdeF", 6)
  output: 32
*/

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	if (!str1 || !str2)
		return (0);
	if (n == 0)
		return (0);
	while (n-- > 1 && *str1 && (*str1 == *str2))
	{
		str1++;
		str2++;
	}
	return (*(unsigned char *) str1 - *(unsigned char *) str2);
}
