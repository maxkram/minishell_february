NAME = minishell
CC = gcc
CFLAGS = -Wall -Werror -Wextra -Ofast # -g -fsanitize=address
LIBS = -Llib/libft -lft -lreadline
INCLUDES = -Iinclude

SRC_DIR := src
OBJ_DIR := obj
LIBFT_DIR := lib/libft

# Default execution directory
EXEC_DIR := old_execution

# Check if the user specified a different execution directory
ifneq ($(NEW_EXEC),)
    EXEC_DIR := new_execution
endif

# Finding source files, excluding the old_execution directory if NEW_EXEC is set
SRC := $(shell find $(SRC_DIR) -name '*.c' ! -path "$(SRC_DIR)/old_execution/*" ! -path "$(SRC_DIR)/new_execution/*")
SRC += $(shell find $(SRC_DIR)/$(EXEC_DIR) -name '*.c')

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
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re tests