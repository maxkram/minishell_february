/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:26:50 by hezhukov          #+#    #+#             */
/*   Updated: 2023/10/30 17:40:36 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
function: memset fills the first n bytes of the memory area
		  pointed to by s with the constant byte c.
   input: ft_memset("abcde", '-', 3)
  output: "---de"
*/

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*current_byte;
	unsigned char	byte_value;

	if (!s)
		return (0);
	current_byte = (unsigned char *)s;
	byte_value = (unsigned char)c;
	while (n--)
	{
		*current_byte = byte_value;
		current_byte++;
	}
	return (s);
}
