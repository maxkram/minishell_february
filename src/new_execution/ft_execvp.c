#include "pipex.h"

char	*build_and_check_exec(const char *dir, const char *file, size_t length)
{
	char	*fullpath;

	fullpath = malloc(length);
	if (!fullpath)
	{
		perror("malloc");
		return (NULL);
	}
	ft_strlcpy(fullpath, dir, length);
	ft_strlcat(fullpath, "/", length);
	ft_strlcat(fullpath, file, length);
	if (access(fullpath, X_OK) == 0)
		return (fullpath);
	free(fullpath);
	return (NULL);
}

char	*search_command_in_directories(const char *file, char *path)
{
	char	**directories;
	char	*fullpath;
	int		i;
	size_t	length;

	directories = tokenize_path(path);
	if (!directories)
		return (NULL);
	i = -1;
	while (directories[++i])
	{
		length = ft_strlen(directories[i]) + 1 + ft_strlen(file) + 1;
		fullpath = build_and_check_exec(directories[i], file, length);
		if (fullpath)
			break ;
	}
	free_string_array(&directories);
	return (fullpath);
}

char	*find_command_path(const char *file, char *const envp[])
{
	char	*path;
	char	*command_path;
	int		i;

	i = 0;
	path = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = ft_strdup(envp[i] + 5);
			break ;
		}
		i++;
	}
	if (!path)
	{
		ft_putstr_fd("PATH variable not found\n", 2);
		return (NULL);
	}
	command_path = search_command_in_directories(file, path);
	free(path);
	return (command_path);
}

int	execute(char *fullpath, char *const argv[], char *const envp[])
{
	if (fullpath)
	{
		execve(fullpath, argv, envp);
		perror("execve");
		free(fullpath);
		return (-1);
	}
	else
	{
		ft_putstr_fd("Command not found\n", 2);
		return (-1);
	}
}

int	ft_execvp(const char *command, char *const argv[], char *const envp[])
{
	char	*command_path;
	int		result;

	command_path = find_command_path(command, envp);
	result = execute(command_path, argv, envp);
	if (command_path)
		free(command_path);
	return (result);
}
