/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:10:38 by akhamass          #+#    #+#             */
/*   Updated: 2024/11/25 21:03:57 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../libft/libft.h"
# include "../minishell.h"
# include <unistd.h>

# define TYPE_WORD 0
# define TYPE_REDIR_IN 1
# define TYPE_REDIR_OUT 2
# define TYPE_REDIR_APPEND 3
# define TYPE_HEREDOC 4
# define TYPE_ENV_VAR 5
# define TYPE_PIPE 6
# define TYPE_QUOTED 7

extern int g_status;

t_token	*tokenize_input(char *input, t_env *env_list);
char *append_character(char *value, char c);
int			handle_single_quotes(char *input, int i, t_token **tokens);
int	handle_double_quotes(char *input, int i, t_token **tokens, t_env *env_list);
char	*handle_variable_expansion(char *input, int *i, int in_quotes, t_token **tokens, t_env *env_list);
int			handle_operator(char *input, int i, t_token **tokens);
int handle_word(char *input, int i, t_token **tokens, t_env *env_list);
int			check_syntax(t_token *tokens);
t_token	*create_token(char *value, int type, int expand);
void		add_token(t_token **tokens, t_token *new_token);
void		free_tokens(t_token *tokens);
int			is_operator(char c);
void		print_tokens(t_token *tokens);
int			handle_pipe(int i, t_token **tokens);
int handle_variable_reference(char *input, int i, t_token **tokens, t_env *env_list);
char		*ft_strndup(const char *s, size_t n);
t_cmd_list	*create_cmd_node(void);
t_cmd_list	*init_command_node(t_cmd_list **cmd_list, t_cmd_list **current_cmd);
int			allocate_command_args(t_cmd_list *current_cmd, int arg_count);
void		post_process_command(t_cmd_list *current_cmd, t_env **env_list);
t_cmd_list	*parse_commands(t_token *tokens, t_env **env_list);
void		set_env_variable(t_env **env_list, char *name, char *value);
char		*get_env_variable(t_env *env_list, char *name);
void		add_env_variable(char *arg, t_env **env_list);
void		handle_export(t_cmd_list *cmd, t_env **env_list);
char		*expand_variable(char *arg, int *i, t_env *env_list, char *result);
char		*append_character_main(char *result, char c);
char		*expand_variables(char *arg, t_env *env_list);
int			add_to_files_list(t_cmd_list *cmd, char *filename, int new_count);
int			add_to_files_type(t_cmd_list *cmd, int type, int new_count);
void		add_redirection(t_cmd_list *cmd, char *filename, int type);
int			handle_redirection_w_token(t_token **tokens,
				t_cmd_list *current_cmd);
void		free_cmd_args(char **cmd_args);
void		free_files_list(char **files_list, int files_count);
void		free_files_type(int *files_type);
void		free_cmd_node(t_cmd_list *cmd_node);
void		free_cmd_list(t_cmd_list *cmd_list);
int	proc_com_args(t_token **tok, t_cmd_list *c_cmd, int *arg_i);
int			process_redirections(t_token **tokens, t_cmd_list *current_cmd);
int process_token(char *input, int i, t_token **tokens, t_env *env_list);
int			count_tokens(t_token *tokens);
int			count_arguments(t_token *tokens);
int			detect_operator(char *input, int i,
				char **operator_str, int *operatortype);
void		extract_name_value(char *env_var, char **name, char **value);
t_env		*create_env_node(char *name, char *value);
t_env		*init_env(char **envp);
int handle_quotes_in_word(char *input, int i, t_token **tokens, char quote_char, int expand);

int	process_token_cmd(t_token **tokens, t_cmd_list *curr_cmd, t_env *env_list);
char *append_string(char *original, char *addition);

char	*get_user_input(void);
void print_commands(t_cmd_list *cmd_list);
void cleanup_resources(t_data *data, t_env **env_list, t_cmd_list *list);


char *create_temp_file(char *content);
char *strjoin_with_newline(char *s1, char *s2);
int process_heredoc(t_token **tokens, t_cmd_list *curr_cmd);
char *get_heredoc(char *delimiter);



int accumulate_dollars(char *input, int i, char **dollar_sequence, int *dollar_count);
int handle_special_variable(char *input, int i, int dollar_count, char *dollar_sequence, t_token **tokens);
int handle_valid_variable(char *input, int i, int dollar_count, char *dollar_sequence, t_token **tokens, t_env *env_list);
int handle_invalid_variable(char *input, int i, char *dollar_sequence, t_token **tokens);

char *get_positional_argument(int index, t_env *env_list);
char *ft_strjoin_multi(int count, ...);
int handle_positional_variable(char *input, int i, t_token **tokens, t_env *env_list);

#endif
