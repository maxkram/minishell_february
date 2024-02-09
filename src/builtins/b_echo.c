#include "minishell.h"

// static int	echo_flag(char **args, int argc)
// {
// 	int	flagged;
// 	int i;

// 	i = 1;
// 	flagged = 0;
// 	if (argc >= 3 && ft_strlen(args[1]) >= 2 && args[1][0] == '-')
// 	{
// 		flagged = 1;
// 		while(args[1][i])
// 		{
// 			if (args[1][i] != 'n')
// 				return (0);
// 			i++;
// 		}
// 	}
// 	return (flagged);
// }

// void	built_echo(char **args, int argc, int fd_out)
// {
// 	int	flagged;
// 	int i;

// 	flagged = echo_flag(args, argc);
// 	i = 1 + flagged;
// 	while (i < argc)
// 	{
// 		ft_putstr_fd(args[i], fd_out);
// 		i++;
// 		if (i < argc)
// 			ft_putchar_fd(' ', fd_out);
// 	}
// 	if (flagged)
// 		ft_putchar_fd('$', fd_out);
// 	ft_putchar_fd('\n', fd_out);

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

void	builtin_echo(t_data *data, t_tab_cmd *tab_cmd)
{
	int	new_line;
	int	i;

	new_line = is_flag(tab_cmd->args[1]);
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