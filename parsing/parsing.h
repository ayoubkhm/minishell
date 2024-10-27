#ifndef PARSING_H
# define PARSING_H

# include "../libft/libft.h"
# include "../minishell.h"
# include <ctype.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define TYPE_WORD 0               // Mots , commandes, arguments
# define TYPE_REDIRECTION_INPUT 1  // '<'
# define TYPE_REDIRECTION_OUTPUT 2 // '>'
# define TYPE_REDIRECTION_APPEND 3 // '>>'
# define TYPE_HEREDOC 4            // '<<'
# define TYPE_ENV_VAR 5            // Variables d'environnement (ex: $HOME)
# define TYPE_PIPE 6               // '|'
# define TYPE_QUOTED 7             // Contenu entre guillemets

extern int			g_last_exit_status;

// Prototypes
t_token				*tokenize_input(char *input);
int					handle_single_quotes(char *input, int i, t_token **tokens);
int					handle_double_quotes(char *input, int i, t_token **tokens);
char				*handle_variable_expansion(char *input, int *i,
						int in_quotes, t_token **tokens);
int					handle_operator(char *input, int i, t_token **tokens);
int					handle_word(char *input, int i, t_token **tokens);
int					check_syntax(t_token *tokens);
t_token				*create_token(char *value, int type);
void				add_token(t_token **tokens, t_token *new_token);
void				free_tokens(t_token *tokens);
int					is_operator(char c);
void				print_tokens(t_token *tokens);
int					handle_pipe(int i, t_token **tokens);
int handle_variable_reference(char *input, int i, t_token **tokens);

// Les nouvelles fonctions pour la liste chainée qui gère par commandes
t_cmd_list *create_cmd_node(void);
char **append_file(char **files_list, char *file);
int *append_file_type(int *files_type, int type);
int count_tokens(t_token *tokens);
t_cmd_list *parse_commands(t_token *tokens, t_env_var **env_list);
void print_cmd_list(t_cmd_list *cmd_list);
void free_cmd_list(t_cmd_list *cmd_list);
char *ft_strndup(const char *s, size_t n);
#endif
