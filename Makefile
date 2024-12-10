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
                exec/builtins/builtins_utils2.c \
                exec/builtins/builtins_utils3.c \
                exec/exec.c \
                exec/execbi.c \
                exec/pipe.c \
                exec/redirections.c \
                exec/exec_utils.c \
                exec/exec_utils2.c \
                exec/exec_utils3.c \
                exec/exec_utils4.c \
                main2.c \
                main.c \
                parsing/parsing_tokenizer.c \
                parsing/parsing_redirections.c \
                parsing/parsing_pipes.c \
                parsing/parsing_syntax.c \
                parsing/parsing_utils.c \
                parsing/parsing_memory.c \
                parsing/parsing_display.c \
                parsing/memory_utils.c \
                parsing/parsing_expansion.c \
                parsing/redirection.c \
                parsing/operator_detection.c \
                parsing/variable_extraction.c \
                parsing/token_quotes.c \
                parsing/token_quotes2.c \
                parsing/token_operations.c \
                parsing/indexing_and_validation.c \
                parsing/command.c \
                parsing/command_args.c \
                parsing/command_env.c \
                parsing/environment.c \
                parsing/expand_variable_utils.c \
                parsing/expansion.c \
                parsing/heredoc.c \
                parsing/heredoc2.c \
                parsing/heredoc3.c \
                parsing/heredoc4.c \
                parsing/heredoc5.c \
                parsing/heredoc_utils.c \
                parsing/init_env.c \
                parsing/token_arguments.c \
                parsing/token_redirection.c \
                parsing/token_processing.c \
                parsing/parsing_quotes.c \
                parsing/quote_utils.c \
                parsing/variable_expansion.c \
                parsing/double_quote_processing.c \
                parsing/tokenizer_quotes1.c \
                parsing/tokenizer_quotes2.c \
                parsing/tokenizer_variables1.c \
                parsing/tokenizer_variables2.c \
                parsing/tokenizer_variables3.c \
                parsing/tokenizer_variables4.c \
                parsing/tokenizer_chars.c \
                parsing/tokenizer_chars2.c \
                parsing/tokenizer_chars3.c \
                parsing/tokenizer_brackets.c \
                parsing/tokenizer_token_dispatch1.c \
                parsing/tokenizer_token_dispatch2.c \
                parsing/tokenizer_assignments.c \
                signals/signals.c



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

tester: re
	@if [ ! -d minishell_tester ]; then git clone https://github.com/LucasKuhn/minishell_tester.git; fi
	@cd minishell_tester && ./tester #| grep -v "✅"