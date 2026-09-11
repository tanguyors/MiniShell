# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/08 17:30:08 by lmonsat           #+#    #+#              #
#    Updated: 2025/03/08 17:30:10 by lmonsat          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Variables
NAME = minishell
CC = cc
CFLAGS = -g -Wall -Wextra -Werror
SRC_DIR = src
OBJ_DIR = obj
INCLUDE = include
LIBFT_DIR = include/libft
LIBFT = $(LIBFT_DIR)/libft.a
LDLIBS = -lreadline

# Fichiers source et objets
SRC = main.c \
	lexer/parsing.c lexer/parsing_quotes.c lexer/parsing_args.c \
	lexer/parsing_redirections.c lexer/expand.c \
	parser/linked_list.c parser/print_linked_list.c \
	executor/execution.c executor/cmd_execution.c executor/get_data.c \
	executor/redirections.c executor/redir_heredoc.c \
	executor/child_process.c executor/multi_pipes.c \
	builtins/echo.c builtins/env.c builtins/env2.c \
	builtins/pwd.c builtins/cd.c builtins/exit.c \
	signals/signal.c \
	utils/utils.c utils/utils_2.c utils/utils_3.c utils/utils_4.c \
	utils/is_utils.c utils/is_utils_2.c utils/is_utils_3.c
SRCS = $(addprefix $(SRC_DIR)/, $(SRC))
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

# Colors
B_BLUE = \033[1;36m
B_GREEN = \033[1;32m
B_WHITE = \033[1;37m
RESET = \033[0m

# Règles
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "\n$(B_BLUE)Compiling $(NAME)...$(B_WHITE)\n"
	$(CC) $(CFLAGS) -I $(INCLUDE) -o $(NAME) $(OBJS) $(LIBFT) $(LDLIBS)
	@echo "\n$(B_GREEN)$(NAME) compiled successfully!$(B_WHITE)\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	@echo "\n$(B_BLUE)Cleaning $(NAME) objs and objs directory...$(B_WHITE)\n"
	rm -rf $(OBJ_DIR)
	@echo "\n$(B_BLUE)Cleaning libft directory...$(B_WHITE)\n"
	$(MAKE) -C $(LIBFT_DIR) clean
	@echo "\n$(B_GREEN)Objs files was removed successfully!$(B_WHITE)\n"
	@echo "$(RESET)"

fclean: clean
	@echo "\n$(B_BLUE)Removing executables...$(B_WHITE)\n"
	rm -rf $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "\n$(B_GREEN)All files was removed successfully!$(B_WHITE)\n"
	@echo "$(RESET)"

re: fclean all

.PHONY: all clean fclean re
