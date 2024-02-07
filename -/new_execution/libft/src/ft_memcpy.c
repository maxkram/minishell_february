/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 17:05:53 by hezhukov          #+#    #+#             */
/*   Updated: 2023/10/30 22:38:44 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
function: memcpy function copies characters from the source to the destination
   input: str = "abcde"
		  ft_memcpy(dst, str, 3) // no overlap
		  ft_memcpy(str + 2, str, 3) // overlap
  output: abc 	// no overlap
  output: ababa // overlap
*/

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*dest_ptr;
	const unsigned char	*src_ptr;

	if (n == 0 || dest == src)
		return (dest);
	if (!dest || !src)
		return (NULL);
	dest_ptr = (unsigned char *) dest;
	src_ptr = (const unsigned char *) src;
	while (n--)
		*dest_ptr++ = *src_ptr++;
	return (dest);
}
