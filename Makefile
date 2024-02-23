NAME = minishell
CC = gcc
CFLAGS = -Wall -Werror -Wextra -g
LIBS = -Llib/libft -lft -Lvendor/readline/lib -lreadline
INCLUDES = -Iinclude -Ivendor/readline/include

SRC_DIR := src
OBJ_DIR := obj
LIBFT_DIR := lib/libft
READLINE_DIR := vendor/readline

# Finding all source files in the src directory
SRC := $(shell find $(SRC_DIR) -name '*.c')

# Transforming source paths into object paths
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Keep the libft prerequisite as before
all: libft $(NAME)

# Rule to compile libft remains unchanged
libft:
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJ)
	@$(CC) $(OBJ) $(LIBS) -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re libft
