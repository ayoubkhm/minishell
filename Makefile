# Makefile

NAME = minishell

CC = gcc
#CFLAGS = -Wall -Wextra -Werror -I. -I./libft

SRCS = main.c parsing_tokenizer.c parsing_quotes.c parsing_expansion.c \
       parsing_redirections.c parsing_pipes.c parsing_syntax.c parsing_utils.c \
       parsing_memory.c parsing_display.c

OBJS = $(SRCS:.c=.o)

LIBFT = ./libft/libft.a

# Chemins pour readline (ajuste-les selon ton installation)
READLINE = -L/opt/homebrew/opt/readline/lib -I/opt/homebrew/opt/readline/include -lreadline

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(READLINE)

$(LIBFT):
	make -C libft

# Règle pour compiler les .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re


