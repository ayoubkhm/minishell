# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/11 16:45:54 by gtraiman          #+#    #+#              #
#    Updated: 2024/10/01 21:12:42 by gtraiman         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = minishell
AR          = ar rcs
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -I . -I./libft -g3

PATH_LIBFT  = libft
LIBFT       = ${PATH_LIBFT}/libft.a

# Chemins pour readline (ajuste-les selon ton installation)
READLINE    = -L/opt/homebrew/opt/readline/lib -I/opt/homebrew/opt/readline/include -lreadline

# Sources des fichiers
SRCS        =   exec/builtins/echo.c \
                exec/builtins/env.c \
                exec/builtins/pwd.c \
                exec/builtins/utils.c \
                exec/builtins/unset.c \
                exec/builtins/export.c \
                exec/exec.c \
                exec/exec_utils.c \
                main.c \
                parsing/parsing_tokenizer.c \
                parsing/parsing_quotes.c \
                parsing/parsing_expansion.c \
                parsing/parsing_redirections.c \
                parsing/parsing_pipes.c \
                parsing/parsing_syntax.c \
                parsing/parsing_utils.c \
                parsing/parsing_memory.c \
                parsing/parsing_display.c \

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
