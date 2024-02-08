/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strspn.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 18:09:44 by hezhukov          #+#    #+#             */
/*   Updated: 2024/01/17 17:55:29 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strspn(const char *str1, const char *str2)
{
	const char	*p1;
	const char	*p2;

	p1 = str1;
	while (*p1)
	{
		p2 = str2;
		while (*p2)
		{
			if (*p1 == *p2)
				break ;
			p2++;
		}
		if (!*p2)
			return (p1 - str1);
		p1++;
	}
	return (p1 - str1);
}
