/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strpbrk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 18:20:02 by hezhukov          #+#    #+#             */
/*   Updated: 2023/12/30 19:04:57 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strpbrk(const char *str, const char *accept)
{
	const char	*p_accept;

	while (*str != '\0')
	{
		p_accept = accept;
		while (*p_accept != '\0')
		{
			if (*str == *p_accept)
				return ((char *)str);
			p_accept++;
		}
		str++;
	}
	return (NULL);
}
