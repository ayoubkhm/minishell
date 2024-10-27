NAME        = minishell
AR          = ar rcs
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -I . -I./libft -g3

PATH_LIBFT  = libft
LIBFT       = ${PATH_LIBFT}/libft.a

# Chemins pour readline (ajuste-les selon ton installation)
READLINE    = -L/opt/homebrew/opt/readline/lib -I/opt/homebrew/opt/readline/include -lreadline

# Sources des fichiers
SRCS        =   exec/builtins/cd.c \
                exec/builtins/echo.c \
                exec/builtins/env.c \
                exec/builtins/exit.c \
                exec/builtins/export.c \
                exec/builtins/pwd.c \
                exec/builtins/unset.c \
                exec/builtins/builtins_utils.c \
                exec/exec.c \
                exec/envp_var.c \
                exec/heredoc.c \
                exec/pipe.c \
                exec/redirections.c \
                exec/exec_utils.c \
                main.c \
                parsing/parsing_tokenizer.c \
                parsing/parsing_quotes.c \
                parsing/parsing_redirections.c \
                parsing/parsing_pipes.c \
                parsing/parsing_syntax.c \
                parsing/parsing_utils.c \
                parsing/parsing_memory.c \
                parsing/parsing_display.c \
                parsing/command.c \
                parsing/memory_utils.c \
                parsing/expansion.c \
                parsing/parsing_expansion.c \
                parsing/redirection.c \
                parsing/environment.c \
                parsing/token_processing.c \
                parsing/operator_detection.c \
                parsing/init_env.c \
                signals/signals.c \



OBJS        = ${SRCS:.c=.o}

all: ${LIBFT} ${NAME}

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

${NAME}: ${OBJS}
	$(CC) ${CFLAGS} -o $(NAME) ${OBJS} ${LIBFT} ${READLINE}

${LIBFT}:
	${MAKE} -C ${PATH_LIBFT} > /dev/null

clean:
	rm -f ${OBJS}
	${MAKE} -C ${PATH_LIBFT} clean > /dev/null

fclean: clean
	rm -f ${NAME}
	${MAKE} -C ${PATH_LIBFT} fclean > /dev/null

re: fclean all

.PHONY: all clean fclean re
