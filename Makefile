# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/05 22:20:46 by lmonsat           #+#    #+#              #
#    Updated: 2025/03/05 22:36:34 by lmonsat          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Variables
NAME = minishell
CC = cc
CFLAGS = -g #-Wall -Wextra -Werror
SRC_DIR = src
OBJ_DIR = obj
INCLUDE = include
LIBFT_DIR = include/libft
LIBFT = $(LIBFT_DIR)/libft.a
LDLIBS = -lreadline

# Fichiers source et objets
SRC = main.c parsing.c execution.c signal.c utils.c is_utils.c linked_list.c echo.c env.c env2.c expand.c pwd.c cd.c exit.c \
		cmd_execution.c get_data.c redirections.c redir_heredoc.c child_process.c multi_pipes.c parsing_quotes.c parsing_args.c \
		parsing_redirections.c is_utils_2.c is_utils_3.c utils_2.c utils_3.c print_linked_list.c
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

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

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

