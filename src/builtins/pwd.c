/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: device <device@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:26:20 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/28 19:41:04 by device           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	build_pwd(t_data *pnt)
{
	char	*temp;

	pnt->code_exit = 0;
	temp = getcwd(NULL, 0);
	if (temp)
		printf("%s\n", temp);
	free(temp);
}
