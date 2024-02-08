/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 18:09:49 by hezhukov          #+#    #+#             */
/*   Updated: 2024/01/17 17:56:07 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtok(char *str, const char *delim)
{
	static char	*lasts;
	char		*token;

	if (str == NULL)
		str = lasts;
	str += ft_strspn(str, delim);
	if (*str == '\0')
	{
		lasts = str;
		return (NULL);
	}
	token = str;
	str = ft_strpbrk(token, delim);
	if (str == NULL)
	{
		lasts = token + ft_strlen(token);
	}
	else
	{
		*str = '\0';
		lasts = str + 1;
	}
	return (token);
}
