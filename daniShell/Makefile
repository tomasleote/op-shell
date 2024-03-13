NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I$(LIBFT_DIR)/includes -I$(INC_DIR) -Ireadline
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
SRC_DIR = src
INC_DIR = include
OBJ_DIR = build

SRC_FILES = main.c data.c signals.c \
		environment/env.c environment/utils1.c environment/prompt.c \
		environment/utils.c \
		lexer/lexer.c lexer/utils.c lexer/buffers.c  lexer/lexing.c \
 		parser/data.c parser/heredoc.c parser/parsing.c parser/joining.c \
 		parser/parser.c parser/pipeline.c parser/utils.c parser/utils1.c \
 		parser/utils2.c parser/redirections.c parser/inputs.c parser/outputs.c \
 		parser/expand_environment.c parser/expand_quotes.c \
		executor/executor.c executor/big_executor.c executor/tiny_executors.c \
		executor/builtins.c executor/pwd.c executor/cd.c executor/unset.c \
		executor/export.c executor/export1.c executor/env.c executor/echo.c \
		executor/exit.c executor/utils.c executor/utils1.c executor/utils2.c \
		executor/utils3.c \
		hash/ft_hash_new.c hash/ft_hash_remove_node.c hash/ft_hash_print.c \
		hash/ft_hash_add_node.c hash/ft_hash_get_item.c  hash/hash_utils1.c \
		hash/ft_hash_create_node.c hash/ft_hash_delete.c \
		hash/ft_hash_get_node.c \
		utils/utils.c utils/utils1.c utils/utils2.c utils/errors.c utils/redir_utils1.c \
		utils/redir_utils2.c utils/lex_utils1.c utils/lex_utils2.c \
        utils/command_utils1.c utils/command_utils2.c utils/command_utils3.c
SRCS = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	@echo "Compiling $@"
	@$(CC) $(CFLAGS) -o $@ $^ $(INCLUDES) $(LDFLAGS)
	@echo "Done!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@echo "Compiling $@"
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(LIBFT):
	@make -s -C $(LIBFT_DIR)

clion: CFLAGS += -g -g3 #-fsanitize=address
clion: re

valgrind: clion
	@valgrind --log-file=valgrind.log --track-fds=yes -s --leak-check=full \
		--show-leak-kinds=all --trace-children=yes --suppressions=readline.supp \
		--track-origins=yes ./$(NAME)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

run: clion
	@./$(NAME)

.PHONY: all clean fclean re clion valgrind