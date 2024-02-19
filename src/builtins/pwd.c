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
