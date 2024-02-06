#include "libft.h"

int	ft_putstr(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (*s)
	{
		ft_putchar_fd(*s++, fd);
		i++;
	}
	return (i);
}