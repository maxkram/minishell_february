/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:01:04 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/22 17:13:12 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *src, size_t n)
{
	size_t		len;
	const char	*ptr;
	char		*dup;
	char		*dst;

	len = 0;
	ptr = src;
	while (len < n && *ptr)
	{
		len++;
		ptr++;
	}
	dup = (char *)malloc(len + 1);
	if (!dup)
		return (NULL);
	ptr = src;
	dst = dup;
	while (len > 0)
	{
		*dst++ = *ptr++;
		len--;
	}
	*dst = '\0';
	return (dup);
}
