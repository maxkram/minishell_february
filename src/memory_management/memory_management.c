#include "minishell.h"
/*
void	gc_malloc_protection(void)
{
	int	i;
	i = 0;
	ft_putstr_fd("Memory allocation error!\n", 2);
	gc_free_all();
	if (get_ms()->line)
		free(get_ms()->line);
	if (get_ms()->rl_env)
	{
		while (get_ms()->rl_env[i])
		{
			free(get_ms()->rl_env[i]);
			i++;
		}
		free(get_ms()->rl_env);
	}
	exit(1);
}
void	*gc_calloc(size_t nmemb, size_t size)
{
	t_mblock	*new_mb;
	new_mb = ft_calloc(1, sizeof(t_mblock));
	if (!new_mb)
		gc_malloc_protection();
	new_mb->next_mb = garbage_collector()->next_mb;
	garbage_collector()->next_mb = new_mb;
	new_mb->address = ft_calloc(nmemb, size);
	if (!new_mb->address)
		gc_malloc_protection();
	return (new_mb->address);
}
void	gc_free(void*address)
{
	t_mblock	*gc_ptr;
	t_mblock	*ptr_copy;
	gc_ptr = garbage_collector()->next_mb;
	ptr_copy = garbage_collector();
	while (gc_ptr)
	{
		if (gc_ptr->address == address)
		{
			ptr_copy->next_mb = gc_ptr->next_mb;
			if (gc_ptr->address)
				free(gc_ptr->address);
			free(gc_ptr);
			return ;
		}
		ptr_copy = gc_ptr;
		gc_ptr = gc_ptr->next_mb;
	}
}
void	gc_detach(void*address)
{
	t_mblock	*gc_ptr;
	t_mblock	*ptr_copy;
	gc_ptr = garbage_collector()->next_mb;
	ptr_copy = garbage_collector();
	while (gc_ptr)
	{
		if (gc_ptr->address == address)
		{
			ptr_copy->next_mb = gc_ptr->next_mb;
			free(gc_ptr);
			return ;
		}
		ptr_copy = gc_ptr;
		gc_ptr = gc_ptr->next_mb;
	}
}
void	gc_free_all(void)
{
	t_mblock	*gc_ptr;
	t_mblock	*ptr_copy;
	gc_ptr = garbage_collector()->next_mb;
	while (gc_ptr)
	{
		if (gc_ptr->address)
			free(gc_ptr->address);
		ptr_copy = gc_ptr;
		gc_ptr = gc_ptr->next_mb;
		free(ptr_copy);
	}
	garbage_collector()->next_mb = NULL;
}


*/
