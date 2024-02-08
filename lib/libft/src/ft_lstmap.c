/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 20:00:40 by hezhukov          #+#    #+#             */
/*   Updated: 2023/11/03 14:54:25 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
input: ft_lstmap(lst, f, del)
function: iterate through lst and apply f to each element and return a new list
output: new list with f applied to each element

void * (return type) – returns a pointer to void
(*f) – f is a pointer to a function.
(void *) – takes one argument: a pointer to void.
void (*del)(void *):

void (return type) – the function returns nothing.
(*del) – indicates that del is a pointer to a function.
(void *) – the function takes one argument: a pointer to void.
*/

t_list	*ft_lstmap(t_list *lst, void *(*f)(void*), void (*del)(void *))
{
	t_list	*first;
	t_list	*new;

	if (!f || !del)
		return (NULL);
	first = NULL;
	while (lst)
	{
		new = ft_lstnew((*f)(lst->content));
		if (!new)
			ft_lstclear(&first, del);
		ft_lstadd_back(&first, new);
		lst = lst->next;
	}
	return (first);
}
