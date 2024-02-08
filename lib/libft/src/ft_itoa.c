/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 17:39:44 by hezhukov          #+#    #+#             */
/*   Updated: 2023/10/30 17:34:08 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
function: convert integer to string
   input: ft_itoa(-1234)
  output: "-1234"
*/

static size_t	count_digits(int n)
{
	size_t	i;

	i = 1;
	while (n)
	{
		n /= 10;
		if (n)
			i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char		*output_string;
	size_t		number_length;
	long int	input_number;

	input_number = n;
	number_length = count_digits(n);
	if (n < 0)
	{
		input_number *= -1;
		number_length++;
	}
	output_string = (char *)malloc(sizeof(char) * (number_length + 1));
	if (!output_string)
		return (NULL);
	output_string[number_length] = 0;
	while (number_length--)
	{
		output_string[number_length] = input_number % 10 + '0';
		input_number /= 10;
	}
	if (n < 0)
		output_string[0] = '-';
	return (output_string);
}
