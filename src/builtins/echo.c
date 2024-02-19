#include "minishell.h"

// static int	echo_flag(char *args)
// {
// 	int	flagged;
// 	int i;

// 	i = 1;
// 	flagged = 0;
// 	if (args[i] && args && args[0] == '-')
// 	{
// 		flagged = 1;
// 		while(args[i])
// 		{
// 			if (args[i] != 'n')
// 				return (0);
// 			i++;
// 		}
// 	}
// 	return (flagged);
// }

// void	built_echo(t_data *pnt, t_tab_cmd *tab_cmd)
// {
// 	int	flagged;
// 	int i;

// 	flagged = echo_flag(tab_cmd->args[1]);
// 	i = 1;
// 	while (echo_flag(tab_cmd->args[i]) == 0)
// 		i++;
// 	while (i < tab_cmd->num_args)
// 	{
// 		ft_putstr_fd(tab_cmd->args[i], STDOUT_FILENO);
// 		printf("test\n");
// 		if (i != tab_cmd->num_args - 1)
// 			ft_putstr_fd(" ", STDOUT_FILENO);
// 		i++;
// 	}
// 	if (flagged)
// 		ft_putstr_fd("\n", STDOUT_FILENO);
// 	pnt->code_exit = 0;
// }

int	is_flag(char *arg)
{
	if (!arg || !*arg || *arg != '-')
		return (1);
	arg++;
	while (*arg)
	{
		if (*arg != 'n')
			return (1);
		arg++;
	}
	return (0);
}

void	built_echo(t_data *data, t_tab_cmd *tab_cmd)
{
	int	new_line;
	int	i;

	new_line = is_flag(tab_cmd->args[1]);
	// printf("%d\n", new_line);
	i = 1;
	while (is_flag(tab_cmd->args[i]) == 0)
		i++;
	while (i < tab_cmd->num_args)
	{
		ft_putstr_fd(tab_cmd->args[i], STDOUT_FILENO);
		if (i != tab_cmd->num_args - 1)
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (new_line)
		ft_putstr_fd("\n", STDOUT_FILENO);
	data->code_exit = 0;
}
