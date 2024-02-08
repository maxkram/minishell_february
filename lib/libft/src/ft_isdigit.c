/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:24:17 by hezhukov          #+#    #+#             */
/*   Updated: 2023/11/02 15:18:24 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
function: check if '1' is a digit
   input: ft_isdigit('1')
  output: 1
	note: subtracting '0' from any digit will give us the same digit
		  in decimal system. So if we subtract '0' from any digit and
		  the result is less than 10 then it is a digit.
*/

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}
