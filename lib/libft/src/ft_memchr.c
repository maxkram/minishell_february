/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 19:11:54 by hezhukov          #+#    #+#             */
/*   Updated: 2023/10/30 23:20:27 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
function: memchr is a function that scans the first n bytes of the memory area
		  pointed to by s for the character c and returns a pointer to it.
   input: ft_memchr("abcdef", 'c', 6)
  output: "cdef"

   input: ft_memchr("abcdef", 'c', 2)
  output: NULL
*/

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*string;
	unsigned char		character;

	character = (unsigned char)c;
	string = s;
	if (!s)
		return (NULL);
	while (n--)
	{
		if (*string == character)
			return ((void *)string);
		string++;
	}
	return (NULL);
}
