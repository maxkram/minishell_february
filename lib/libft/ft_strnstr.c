/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 16:06:56 by ycyr-roy          #+#    #+#             */
/*   Updated: 2024/02/07 12:34:48 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *hay, const char *nee, size_t len)
{
	size_t	x;
	size_t	y;

	x = 0;
	if (!*nee)
		return ((char *)hay);
	while (hay[x] != '\0' && x < len)
	{
		y = 0;
		while ((nee[y] == hay[x + y] && x + y < len))
		{
			if (nee[y + 1] == '\0')
				return ((char *)hay + x);
			y++;
		}
		x++;
	}
	return (NULL);
}
