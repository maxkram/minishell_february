/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 20:29:00 by hezhukov          #+#    #+#             */
/*   Updated: 2023/11/03 13:59:51 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
function: locates the first occurrence of the null-terminated string (needle)
		  in the string (haystack), where not more than (len) characters are
		  searched. Characters that appear after a '\0' character are not
		  searched.
   input: ft_strnstr("abcdef", "abc", 5)
  output: "cdef"

   input: ft_strnstr("abcdEabcde", "abcdef", 5)
  output: NULL

    note: len >= needle_len check "room" left in the haystack to fit the needle.
*/

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	needle_len;

	if (!haystack || !needle)
		return (NULL);
	if (*needle == '\0')
		return ((char *)haystack);
	needle_len = ft_strlen(needle);
	while (*haystack && len-- >= needle_len)
	{
		if (ft_strncmp(haystack, needle, needle_len) == 0)
			return ((char *)haystack);
		haystack++;
	}
	return (NULL);
}
