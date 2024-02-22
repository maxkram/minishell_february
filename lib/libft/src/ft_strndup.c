/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:01:04 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/20 12:02:25 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strndup(const char *src, size_t n)
{
    size_t  len;
    const char *ptr;

    len = 0;
    ptr = src;
    while (len < n && *ptr)
    {
        len++;
        ptr++;
    }
    char *dup = (char *)malloc(len + 1);
    if (!dup)
        return (NULL);
    ptr = src;
    char *dst = dup;
    while (len > 0)
    {
        *dst++ = *ptr++;
        len--;
    }
    *dst = '\0';
    return (dup);
}
