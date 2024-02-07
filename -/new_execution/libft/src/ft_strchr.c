/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 20:09:01 by hezhukov          #+#    #+#             */
/*   Updated: 2023/11/01 17:35:58 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
function: locates the first occurrence of (c) in the string (s) and returns
		  a pointer to it.
   input: ft_strchr("Hello World", 'o')
  output: "o World"
*/

char	*ft_strchr(const char *s, int c)
{
	int	i;

	if (!s)
		return (NULL);
	i = 0;
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)&s[i]);
		s++;
	}
	if ((char) c == '\0')
		return ((char *)(s + i));
	return (NULL);
}
