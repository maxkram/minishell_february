/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:26:20 by hezhukov          #+#    #+#             */
/*   Updated: 2024/02/22 18:26:27 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	build_pwd(t_data *data)
{
	char	*temp;

	data->code_exit = 0;
	temp = getcwd(NULL, 0);
	if (temp)
		printf("%s\n", temp);
	free(temp);
}
