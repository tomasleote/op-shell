NAME = shell
CC = gcc
CFLAGS = -Wall -Wall -Werror -g
SRC_DIR = src
OBJ_DIR = build
INC_DIR = headers #inc
INC_FLAGS = -I $(INC_DIR)
SRC_FILES =	main.c \
			shell.c \
			shellData.c \
			utils.c \
			parser/command.c \
			parser/scanner.c \
			parser/parser.c \
			builtIns.c \
			parser/operators.c \
			parser/tokenList.c

SRCS = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

$(NAME) : $(OBJS)
	@echo "Compiling $@"
	@$(CC) $(CFLAGS) -o $@ $^ $(INC_FLAGS)
	@echo "Done!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

all: $(NAME)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re