#ifndef PARSING_H
#define PARSING_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include "./libft/libft.h"

#define TYPE_WORD 0                 // Mots , commandes, arguments
#define TYPE_REDIRECTION_INPUT 1    // '<'
#define TYPE_REDIRECTION_OUTPUT 2   // '>'
#define TYPE_REDIRECTION_APPEND 3   // '>>'
#define TYPE_HEREDOC 4              // '<<'
#define TYPE_ENV_VAR 5              // Variables d'environnement (ex: $HOME)
#define TYPE_PIPE 6                 // '|'
#define TYPE_QUOTED 7               // Contenu entre guillemets



extern int g_last_exit_status;

typedef struct s_token
{
    char *value;
    int type;
    struct s_token *next;
    struct s_token *prev;
} t_token;

// Prototypes
t_token *tokenize_input(char *input);
int handle_single_quotes(char *input, int i, t_token **tokens);
int handle_double_quotes(char *input, int i, t_token **tokens);
char *handle_variable_expansion(char *input, int *i, int in_quotes, t_token **tokens);
int handle_operator(char *input, int i, t_token **tokens);
int handle_word(char *input, int i, t_token **tokens);
int check_syntax(t_token *tokens);
t_token *create_token(char *value, int type);
void add_token(t_token **tokens, t_token *new_token);
void free_tokens(t_token *tokens);
int is_operator(char c);
void print_tokens(t_token *tokens);

#endif
