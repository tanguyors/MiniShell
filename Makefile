# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: root <root@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/28 10:00:09 by root              #+#    #+#              #
#    Updated: 2025/03/02 16:01:38 by root             ###   ########.fr        #
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
SRC = main.c parsing.c execution.c builtins.c signal.c utils.c is_utils.c linked_list.c echo.c env.c env2.c expand.c pwd.c cd.c
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
	@if [ ! -f $(NAME) ]; \
	then \
		echo "\n$(B_BLUE)Compiling $(NAME)...$(B_WHITE)\n"; \
		echo "$(CC) $(CFLAGS) -I $(INCLUDE) -o $(NAME) $(OBJS) $(LIBFT) $(LDLIBS)"; \
		$(CC) $(CFLAGS) -I $(INCLUDE) -o $(NAME) $(OBJS) $(LIBFT) $(LDLIBS); \
		echo "\n$(B_GREEN)$(NAME) compiled successfully!$(B_WHITE)\n"; \
	fi

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

