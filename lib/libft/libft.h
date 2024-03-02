/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezhukov <hezhukov@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:10:48 by hezhukov          #+#    #+#             */
/*   Updated: 2023/10/16 12:10:48 by hezhukov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <stddef.h>
# include <stdint.h>
# include <string.h>
# include <limits.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <assert.h>
# include <math.h>
# include <ctype.h>
# include <stdarg.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}				t_list;

int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
size_t	ft_strlen(const char *s);
void	*ft_memset(void *str, int c, size_t n);
void	ft_bzero(void *s, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dst, const void *src, size_t n);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
int		ft_toupper(int c);
int		ft_tolower(int c);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
int		ft_atoi(const char *nptr);
void	*ft_calloc(size_t num, size_t size);
char	*ft_substr(const char *s, unsigned int start, size_t len);
char	*ft_strdup( const char *str1 );
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	*ft_itoa(int nb);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
// void	ft_putchar_fd(char c, int fd);
int		ft_putchar_fd(char c, int fd);
// void	ft_putstr_fd(char *s, int fd);
int		ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
int		ft_islower(int c);
t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
char	*ft_strtok(char *str, const char *delim);
size_t	ft_strspn(const char *str1, const char *str2);
size_t	ft_strcspn(const char *str1, const char *str2);
char	*ft_strpbrk(const char *str, const char *accept);
char	*ft_strndup(const char *src, size_t n);

char	*charjoinfree(char *str, char c);
int		ft_printf_fd(int fd, const char *format, ...);
int		find_symbol(char c, char *str);
char	*ft_strdup_fd(char *src);
int		arr_len(char **arr);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strcat(const char *s1, const char *s2);
char	*ft_strstr(const char *s1, const char *s2);
char	**ft_split_fd(char const *s, char c);
int		ft_putstr(char *s, int fd);
int		ft_split_length(char **arr);
void	*free_null(void *ptr);
void	*arr_free(void **arr);
int		ft_putchar(char c, int fd);
int		ft_putstr(char *s, int fd);
int		count_dig(int n);
int		ft_putnbr(int n, int fd);

#endif
