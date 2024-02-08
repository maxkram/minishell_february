# for a compilation with new execution directory, run `make NEW_EXEC=1`

NAME = minishell
CC = gcc
CFLAGS = -Wall -Werror -Wextra -Ofast # -g -fsanitize=address
LIBS = -Llib/libft -lft -Lvendor/readline/lib -lreadline
INCLUDES = -Iinclude -Ivendor/readline/include

SRC_DIR := src
OBJ_DIR := obj
LIBFT_DIR := lib/libft
READLINE_DIR := vendor/readline

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

# Add libft as a prerequisite to the all target or to the $(NAME) target
all: libft $(NAME)

# Rule to compile libft
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

.PHONY: all clean fclean re tests libft
