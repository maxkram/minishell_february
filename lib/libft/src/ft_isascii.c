/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 14:24:53 by hezhukov          #+#    #+#             */
/*   Updated: 2023/11/02 15:19:29 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
function: check if 'a' is an ascii character
   input: ft_isascii('a')
  output: 1
*/

int	ft_isascii(int c)
{
	return (c >= 0 && c <= 127);
}
