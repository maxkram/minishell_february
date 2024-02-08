/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 17:35:15 by hezhukov          #+#    #+#             */
/*   Updated: 2023/11/03 15:13:52 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_split(char **arr, int n)
{
	while (n--)
		free(arr[n]);
	free(arr);
}

static int	count_words(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s && *s != c)
			s++;
	}
	return (count);
}

static char	*allocate_word(char *start, char *end)
{
	int		i;
	char	*word;

	i = 0;
	word = malloc(end - start + 1);
	if (!word)
		return (NULL);
	while (start + i < end)
	{
		word[i] = start[i];
		i++;
	}
	word[end - start] = '\0';
	return (word);
}

static void	fill_array(char **arr, char const *s, char c)
{
	char	*start;
	int		i;

	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			start = (char *)s;
			while (*s && *s != c)
				s++;
			arr[i] = allocate_word(start, (char *)s);
			if (!arr[i])
			{
				free_split(arr, i);
				return ;
			}
			i++;
		}
	}
	arr[i] = NULL;
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	int		word_count;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	arr = malloc((word_count + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	fill_array(arr, s, c);
	return (arr);
}
