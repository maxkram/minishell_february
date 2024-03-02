NAME = minishell
CC = gcc
CFLAGS = -Wall -Werror -Wextra -g
LIBS = -Llib/libft -lft -L vendor/readline/lib -lreadline -lncurses
INCLUDES = -Iinclude -I$(READLINE_PATH)/include

SCRIPT_URL=https://gist.githubusercontent.com/GlaceCoding/febe3306c2a9ec2b0f8df44fc35aecdd/raw/2aeef98aa0adbb305c1768ae29a842081372dd7a/install_readline.sh
SCRIPT_PATH=$(LIB_FOLDER)/install_readline.sh
LIB_FOLDER=vendor
READLINE_PATH=$(LIB_FOLDER)/readline

SRC_DIR := src
OBJ_DIR := obj
LIBFT_DIR := lib/libft
READLINE_DIR := vendor/readline

SRC := $(shell find $(SRC_DIR) -name '*.c')
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Update `all` target to include `install_readline` as a dependency
all: install_readline libft $(NAME)

install_readline:
	@echo "Ensuring vendor directory exists..."
	@mkdir -p $(LIB_FOLDER)
	@if [ ! -f "$(SCRIPT_PATH)" ]; then \
		echo "Downloading readline installation script..."; \
		curl -L $(SCRIPT_URL) -o $(SCRIPT_PATH); \
	fi
	@if [ ! -d "$(READLINE_PATH)/lib" ] || [ ! -f "$(READLINE_PATH)/lib/libreadline.a" ]; then \
		echo "Executing readline installation script..."; \
		chmod +x $(SCRIPT_PATH); \
		$(SCRIPT_PATH); \
	else \
		echo "Readline library is already installed."; \
	fi

libft:
	@echo "Compiling $(NAME)..."
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

.PHONY: all clean fclean re libft install_readline
