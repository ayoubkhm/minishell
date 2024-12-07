/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:10:38 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 17:13:11 by akhamass         ###   ########.fr       */
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


t_token	*tokenize_input(char *input, t_env *env_list);
char *append_character(char *value, char c);
char *handle_single_quotes(char *input, int *i, t_env *env_list);
char	*gere_duo_quote(char *input, int *i, t_token **tokens, t_env *env_list);
char *handle_var_expansion(t_var_exp *ctx);
int			handle_operator(char *input, int i, t_token **tokens);
int			check_syntax(t_token *tokens);
t_token	*create_token(char *value, int type, int expand);
void		add_token(t_token **tokens, t_token *new_token);
void		free_tokens(t_token *tokens);
int			is_operator(char c);
void		print_tokens(t_token *tokens);
int			handle_pipe(int i, t_token **tokens);
int gere_var_ref(char *input, int i, t_token **tokens, t_env *env_list);
char		*ft_strndup(const char *s, size_t n);
t_cmd_list	*create_cmd_node(void);
t_cmd_list	*init_command_node(t_cmd_list **cmd_list, t_cmd_list **current_cmd);
int			allocate_command_args(t_cmd_list *current_cmd, int arg_count);
void		post_process_command(t_cmd_list *current_cmd, t_env **env_list);
t_cmd_list *parse_commands(t_token *tokens, t_env **env_list);
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
int			count_tokens(t_token *tokens);
int			count_arguments(t_token *tokens);
int			detect_operator(char *input, int i,
				char **operator_str, int *operatortype);
void		extract_name_value(char *env_var, char **name, char **value);
t_env		*create_env_node(char *name, char *value);
t_env		*init_env(char **envp);
int handle_quotes_in_word(char *input, int i, t_token **tokens, char quote_char, int expand, t_env *env_list);

int process_token_cmd(t_token **tokens, t_cmd_list *curr_cmd);
char *append_string(char *original, char *addition);

char	*get_user_input(void);
void print_commands(t_cmd_list *cmd_list);
void cleanup_resources(t_data *data, t_env **env_list, t_cmd_list *list);


char *create_temp_file(char *content);
char *strjoin_with_newline(char *s1, char *s2);
int process_heredoc(t_token **tokens, t_cmd_list *curr_cmd);
char *get_heredoc(char *delimiter);


int accumulate_dollars(char *input, int i, char **dollar_sequence, int *dollar_count);
int handle_special_variable(char *input, int i, t_token **tokens, t_env *env_list, char *dollar_sequence);
int gere_valid_var(t_ctx *ctx);
int handle_invalid_variable(t_ctx *ctx);
char *get_positional_argument(int index, t_env *env_list);
char *ft_strjoin_multi(int count, ...);
int handle_positional_variable(char *input, int i, t_token **tokens, t_env *env_list);


int parse_input(char *input, t_token **tokens, t_env *env_list);
int parse_word(char *input, int i, t_token **tokens, t_env *env_list);



//variable_extraction.c
char	*extract_variable_value(char *input, int *i, t_env *env_list);
char	**xtrct_split_var_val(char *inp, int *i, t_env *e_l, char *dol_seq);
char	*bild_cmbined_val(int dol_cnt, char *dol_seq, char *var_val);
int	append_special_chars(char *input, int i, char **combined_value);
void	free_split_values(char **split_values);


//token_quotes.c
int	handle_quotes_in_var(char *inp, int i, char **token_value);
char	*get_token_value(t_ctx *ctx, char **split_values, int j);
int	process_first_token(t_ctx *ctx, int i, char **token_value);
void	finalize_first_token(t_ctx *ctx);
int	handle_tok_proc(t_ctx *ctx, char **split_values, int j, int i);

//token_operations.c
int	add_all_tokens(t_ctx *ctx, char **split_values, int i);
int	gere_valid_var(t_ctx *ctx);
int	cree_et_add_tok_lit(t_ctx *ctx, int start, int end);
int	handle_invalid_variable(t_ctx *ctx);

//indexing_and_validation.c
int	validate_start_index(t_ctx *ctx, int start);
int	find_end_index(t_ctx *ctx);

//command.c
t_cmd_list	*create_cmd_node(void);
t_cmd_list	*init_command_node(t_cmd_list **cmd_list, t_cmd_list **current_cmd);
void	post_process_command(t_cmd_list *current_cmd, t_env **env_list);
int	p_singl_c(t_cmd_list **cmd_l, t_cmd_list **c_c, t_token **tok, t_env **e_l);
t_cmd_list	*parse_commands(t_token *tokens, t_env **env_list);

//command.arg.c
int	allocate_command_args(t_cmd_list *current_cmd, int arg_count);
void	remove_empty_arguments(t_cmd_list *current_cmd);
int	count_non_empty_arguments(char **cmd_args);
char	**allocate_arguments_array(int new_arg_count);
char	**filter_non_empty_arg(char **cmd_args, int new_arg_count);

//command_env.c
void	remove_env_variable(char *var_name, t_env **env_list);
void	handle_unset(t_cmd_list *cmd, t_env **env_list);

//environment.c
void	populate_arguments_array(char **cmd_args, char **new_cmd_args);
void	set_env_variable(t_env **env_list, char *name, char *value);
char	*get_env_variable(t_env *env_list, char *name);
void	add_env_variable(char *arg, t_env **env_list);
void	handle_export(t_cmd_list *cmd, t_env **env_list);

//expand_variable_utils.c
char	*expand_variable(char *arg, int *i, t_env *env_list, char *result);
char	*append_character_main(char *result, char c);
char	*append_string(char *original, char *addition);

//expansion.c
int	handle_exit_status(t_env *env_list, char **result);
int	append_character_main_and_check(char **result, char c);
int	expand_variable_and_check(char *arg, int *i, t_env *env_list, char **result);
int	handle_special_case(char *arg, int *i, t_env *env_list, char **result);
char	*expand_variables(char *arg, t_env *env_list);

//heredoc.c
char	*create_temp_file(char *content);
char	*get_heredoc(char *delimiter);
int	update_cmd_files(t_cmd_list *curr_cmd, char *temp_filename);
int	handle_heredoc(t_token *current, t_cmd_list *curr_cmd, char **temp_filename);
int	process_heredoc(t_token **tokens, t_cmd_list *curr_cmd);

//heredoc_utils.c
int	create_unique_temp_file(const char *base_filename, char *filename);
char	*allocate_filename(const char *base_filename);
int	write_to_temp_file(int fd, const char *filename, const char *content);
char	*strjoin_with_newline(char *s1, char *s2);
size_t	calculate_total_length(char *s1, char *s2);

//init_env.c
void	extract_name_value(char *env_var, char **name, char **value);
t_env	*create_env_node(char *name, char *value);
t_env	*init_env(char **envp);

//memory_utils.c
void	free_cmd_args(char **cmd_args);
void	free_files_list(char **files_list, int files_count);
void	free_files_type(int *files_type);
void	free_cmd_node(t_cmd_list *cmd_node);
void	free_cmd_list(t_cmd_list *cmd_list);

//operation_detection.c
int	is_operator(char c);
int	detect_specific_oper(char *input, int i, char **oper_str, int *oper_type);
int	detect_operator(char *input, int i, char **operator_str, int *operatortype);

//parsing_display.c
void	print_tokens(t_token *tokens);

//parsing_expansion.c
char	*extract_variable_name(char *input, int *i);
char	*handle_var_expansion(t_var_exp *ctx);

//parsing_memory.c
void	free_tokens(t_token *tokens);

//parsing_pipes.c
int	handle_pipe(int i, t_token **tokens);

//parsing_quotes.c
int	parse_input(char *input, t_token **tokens, t_env *env_list);
int	parse_word(char *input, int i, t_token **tokens, t_env *env_list);
char	*parse_character(char *input, int *i, t_token **tokens, t_env *env_list);
char	*handle_single_quotes(char *input, int *i, t_env *env_list);
char	*gere_duo_quote(char *input, int *i, t_token **tokens, t_env *env_list);



//quotes_utils.c
char	*join_and_free(char *value, char *temp);
char	*append_character(char *value, char c);
char	*append_and_free(char *value, char *to_append);
int	is_valid_closing_quote(char *input, int *i, t_env *env_list, char *value);
int	is_valid_variable_start(char c);


//variable_expansion.c
char	*append_variable_value(char *value, t_var_exp *ctx);
char	*process_invalid_variable(t_var_exp *ctx, char *value);
char	*process_exit_status(t_var_exp *ctx, char *value);
char	*process_dollar_sign(t_var_exp *ctx, char *value);
char	*handle_quoted_exit_status(t_env *env_list, int *index);

//double_quote_processing.c
char	*process_regular_character(t_var_exp *ctx, char *value);
char	*process_double_quote_characters(t_var_exp *ctx, char *value);
char	*process_double_quote_content(t_var_exp *ctx);
int	init_var_exp_struc(t_var_exp *ctx, t_var_exp_params *params);

//redirection.c
int	add_to_files_list(t_cmd_list *cmd, char *filename, int new_count);
int	add_to_files_type(t_cmd_list *cmd, int type, int new_count);
void	add_redirection(t_cmd_list *cmd, char *filename, int type);
int	handle_redirection_w_token(t_token **tokens, t_cmd_list *current_cmd);

//token_arguments.c
int	proc_com_args(t_token **tok, t_cmd_list *c_cmd, int *arg_i);
int	count_arguments(t_token *tokens);
int	count_tokens(t_token *tokens);

//token_processing.c
void	skip_token(t_token **tokens);
int	handle_argument_tokens(t_token **tok, t_cmd_list *curr_cmd, int *arg_index);
int	handle_redirection_tokens(t_token **tokens, t_cmd_list *curr_cmd);
int	process_token_cmd(t_token **tokens, t_cmd_list *curr_cmd);

//token_redirection.c
int	process_redirections(t_token **tokens, t_cmd_list *current_cmd);
int	process_command_args(t_token **tokens, t_cmd_list *curr_cmd, int *arg_index);
int	process_command_redirections(t_token **tokens, t_cmd_list *curr_cmd);

//variable_extraction.c
char	*extract_variable_value(char *input, int *i, t_env *env_list);
char	**xtrct_split_var_val(char *inp, int *i, t_env *e_l, char *dol_seq);
char	*bild_cmbined_val(int dol_cnt, char *dol_seq, char *var_val);
int	append_special_chars(char *input, int i, char **combined_value);
void	free_split_values(char **split_values);

// parsing_tokenizer.c
t_token *tokenize_input(char *input, t_env *env_list);
int	process_token(char *input, int i, t_token **tokens, t_env *env_list);
int	handle_word(char *input, int i, t_token **tokens, t_env *env_list);
int gere_var_ref(char *inp, int i, t_token **tok, t_env *e_l);
int handle_quotes(t_ctx *ctx);


// tokenizer_quotes1.c
char *extract_quoted_content(char *input, int *i, char quote_type);
char *xpnd_or_cp_cntnt(char *quoted_cntnt, char quote_type, t_env *e_l);
char *safe_concat(char *final_content, char *new_content);
char *gere_adjcnt_txt(char *input, int *i, char *final_content);
char *process_quoted_section(t_quote_context *ctx, char quote_type);

// tokenizer_quotes2.c
char *process_all_quotes(t_quote_context *ctx);
int gere_var_quotes(char *input, int i, t_token **tokens, t_env *env_list);
int check_and_gere_quotes(char *input, int i, t_token **tok, t_env *env_list);
int is_valid_closing_quote(char *input, int *i, t_env *env_list, char *value);
char *remove_quotes(const char *input);

// tokenizer_variables1.c
int	gere_var2(t_pars_cxt *ctx);
int handle_variable_type(t_ctx *ctx);
int	gere_spcial_var(t_ctx *ctx);

// tokenizer_variables2.c
char *initialize_var_value(char *inp, int i, t_env *e_l);
char *accumulate_invalid_chars(char *inp, int *i, char *token_value);
int finalize_and_add_tok(t_token **tok, char *tok_val, char *dol_seq, int i);
int	handle_dollar_sequence(t_ctx *ctx);

// tokenizer_variables1.c
int gere_num_var(t_ctx *ctx);
int	accumulate_dol_seq(char *inp, int i, char **dol_seq, int *dol_cnt);

// tokenizer_chars.c
int handle_backslash(char *input, int i, char **current_value);
int handle_character_iteration(char *input, int i);
char *extract_prefix(char *input, int start, int i);
void process_prefix_as_word(char *prefix, t_token **tokens);
int	handle_regular_characters(char *inp, int i, t_token **tok, t_env *env_list);

// tokenizer_brackets.c
char *extract_and_expand(char *input, int *i, t_env *env_list);
void create_and_add_token(t_token **tok, char *xpndd_val, char is_closed);
int handle_brackets(char *input, int i, t_token **tokens, t_env *env_list);
t_token *get_last_token(t_token *tokens);
void remove_last_token(t_token **tokens);

// tokenizer_token_dispatch1.c
int	handle_variable_token(char *input, int i, t_token **tokens, t_env *env_list);
int	handle_operator_token(char *input, int i, t_token **tokens);
int	handle_quotes_token(char *input, int i, t_token **tokens, t_env *env_list);
int	handle_brackets_token(char *input, int i, t_token **tokens, t_env *env_list);
int	handle_backslash_token(char *input, int i, t_token **tokens);

// tokenizer_token_dispatch2.c
int	proc_special_tok(char *input, int i, t_token **tokens, t_env *env_list);
int	proc_complex_tok(char *input, int i, t_token **tokens, t_env *env_list);
int handle_operator(char *input, int i, t_token **tokens);
int add_variable_token(char *name, char *value, t_token **tokens, int i);
int extract_indices(char *input, int i, t_variable_data *data);

// tokenizer_assignments.c
int extract_name_and_val(char *inp, t_variable_data *data);
int add_var_token_and_cleanup(t_variable_data *data, t_token **tokens, int i);
int parse_variable_assignment(char *input, int i, int start, t_token **tokens);


#endif
