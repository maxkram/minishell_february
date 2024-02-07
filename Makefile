NAME = minishell
CC = gcc
CFLAGS = -Wall -Werror -Wextra -Ofast # -g -fsanitize=address
LIBS = -Llib/libft -lft -lreadline
INCLUDES = -Iinclude

SRC_DIR := src
OBJ_DIR := obj
LIBFT_DIR := lib/libft

# Finding source files in subdirectories
SRC := $(shell find $(SRC_DIR) -name '*.c')

# Transforming source paths into object paths
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJ)
	@$(CC) $(OBJ) $(LIBS) -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re tests
