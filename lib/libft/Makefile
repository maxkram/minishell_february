# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: device <device@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/17 17:24:38 by hezhukov          #+#    #+#              #
#    Updated: 2024/02/28 18:57:51 by device           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
SRCDIR = src
OBJDIR = obj

# Find all .c files in SRCDIR, including main and bonus source files
SRCS = $(shell find $(SRCDIR) -name "*.c")

OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

INCLUDES = -I.
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
RM = /bin/rm -f

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@ranlib $(NAME)

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
