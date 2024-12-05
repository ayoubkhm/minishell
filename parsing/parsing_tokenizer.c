#include "parsing.h"

int handle_backslash(char *input, int i, char **current_value)
{
    if (!input[i + 1]) // Vérifie si le backslash est à la fin
    {
        fprintf(stderr, "minishell: syntax error: backslash at end of input\n");
        return -1;
    }

    // Préparer le caractère échappé
    char escaped_char[2] = {input[i + 1], '\0'};
    char *temp = *current_value;
    *current_value = ft_strjoin(*current_value, escaped_char);
    free(temp);
    i += 2; // Avance après le backslash et le caractère suivant

    // Continuer à collecter les caractères littéraux suivants
    while (input[i] && !isspace(input[i]) && !is_operator(input[i]))
    {
        char literal_char[2] = {input[i], '\0'};
        temp = *current_value;
        *current_value = ft_strjoin(*current_value, literal_char);
        free(temp);
        i++;
    }

    return i; // Retourne l'index après les caractères littéraux collectés
}


int handle_brackets(char *input, int i, t_token **tokens, t_env *env_list)
{
    int start = ++i; // Avance après le crochet ouvrant '['
    char *value;

    while (input[i] && input[i] != ']') // Recherche le crochet fermant ']'
        i++;

    // Extrait le contenu entre les crochets
    value = ft_strndup(&input[start], i - start);

    // Effectue l'expansion des variables sur le contenu
    char *expanded_value = expand_variables(value, env_list);
    free(value);

    char *token_value;

    if (input[i] == ']') // Si un crochet fermant existe
    {
        // Reconstruit le token avec les crochets inclus
        char *temp = ft_strjoin("[", expanded_value);
        token_value = ft_strjoin(temp, "]");
        free(temp);
    }
    else // Sinon, garde uniquement le contenu entre crochets
    {
        token_value = ft_strjoin("[", expanded_value);
    }

    free(expanded_value);

    // Ajoute le token
    add_token(tokens, create_token(token_value, TYPE_WORD, 0));
    free(token_value);

    return input[i] == ']' ? i + 1 : i; // Avance après le crochet fermant si présent
}


char *remove_quotes(const char *input)
{
    char *cleaned = malloc(strlen(input) + 1);
    if (!cleaned)
    {
        fprintf(stderr, "minishell: memory allocation failed in remove_quotes\n");
        return NULL;
    }

    int j = 0;
    for (int i = 0; input[i]; i++)
    {
        if (input[i] != '\'' && input[i] != '"') // Ignorer les quotes
        {
            cleaned[j++] = input[i];
        }
    }
    cleaned[j] = '\0';

    return cleaned;
}

int handle_character_iteration(char *input, int i)
{
    while (input[i] && !isspace(input[i]) && !is_operator(input[i]))
    {
        if (input[i] == '\'' || input[i] == '"') // Ignorer les quotes
            i++;
        else
            i++;
    }
    return i;
}

char *extract_prefix(char *input, int start, int i)
{
    if (start < i)
        return ft_substr(input, start, i - start);
    return NULL;
}

int handle_variable2(char *input, int i, t_token **tokens, t_env *env_list, char **prefix)
{
    i = handle_variable_reference(input, i, tokens, env_list);

    if (*tokens == NULL || (*tokens)->value == NULL) // Vérifier les erreurs
    {
        fprintf(stderr, "minishell: command not found\n");
        free(*prefix);
        *prefix = NULL;
        return i;
    }

    if (*prefix && **prefix) // Combiner le préfixe avec la valeur de la variable
    {
        char *var_value = (*tokens)->value;
        char *combined = ft_strjoin(*prefix, var_value);
        free(*prefix);
        free(var_value);
        (*tokens)->value = combined;
    }
    return i;
}

void process_prefix_as_word(char *prefix, t_token **tokens)
{
    if (prefix && *prefix) // Si un préfixe existe
    {
        char *cleaned_prefix = remove_quotes(prefix); // Supprimer les quotes
        free(prefix); // Libérer l'ancien préfixe
        add_token(tokens, create_token(cleaned_prefix, TYPE_WORD, 1));
        free(cleaned_prefix);
    }
}

int handle_regular_characters(char *input, int i, t_token **tokens, t_env *env_list)
{
    int start = i;

    // Avancer jusqu'à un caractère spécial ou espace
    i = handle_character_iteration(input, i);

    // Extraire le préfixe
    char *prefix = extract_prefix(input, start, i);

    // Gérer une variable si '$' est rencontré
    if (input[i] == '$')
    {
        i = handle_variable2(input, i, tokens, env_list, &prefix);
    }
    else // Sinon, traiter le préfixe comme un mot
    {
        process_prefix_as_word(prefix, tokens);
    }

    return i;
}





t_token *get_last_token(t_token *tokens)
{
    if (!tokens)
        return NULL;

    while (tokens->next)
        tokens = tokens->next;

    return tokens;
}


void remove_last_token(t_token **tokens)
{
    if (!tokens || !*tokens)
        return;

    t_token *current = *tokens;
    if (!current->next)
    {
        free(current->value);
        free(current);
        *tokens = NULL;
        return;
    }
    while (current->next && current->next->next)
    {
        current = current->next;
    }
    free(current->next->value);
    free(current->next);
    current->next = NULL;
}


int process_token(char *input, int i, t_token **tokens, t_env *env_list)
{
    char *current_value = NULL;

    if (input[i] == '$')
    {
        return handle_variable_reference(input, i+1, tokens, env_list);
    }

    // Cas 2 : Si c'est un opérateur
    if (is_operator(input[i]))
    {
        return handle_operator(input, i, tokens);
    }

    // Cas 3 : Si c'est une quote suivie d'un $
    if ((input[i] == '\'' || input[i] == '"') && input[i + 1] == '$')
    {
        return handle_variable_reference(input, i, tokens, env_list);
    }

    // Cas 4 : Si c'est une quote simple ou double (non suivie d'un $)
    if (input[i] == '\'' || input[i] == '"')
    {
        int new_i = parse_word(input, i, tokens, env_list);
        if (new_i == -1)
        {
            return -1;
        }
        return new_i; // Retourner le nouvel index avancé après les quotes
    }

    // Cas 5 : Si c'est un bracket [
    if (input[i] == '[')
    {
        return handle_brackets(input, i, tokens, env_list);
    }

    // Cas 6 : Si c'est un backslash
    if (input[i] == '\\')
    {
        current_value = ft_strdup("");
        if (!current_value)
        {
            fprintf(stderr, "minishell: memory allocation failed for current_value\n");
            return -1;
        }

        int result = handle_backslash(input, i, &current_value);
        if (result == -1)
        {
            free(current_value);
            return -1;
        }

        add_token(tokens, create_token(current_value, TYPE_WORD, 0));
        free(current_value);
        return result;
    }

    // Cas 7 : Tous les autres cas (mots classiques, etc.)
    return handle_word(input, i, tokens, env_list);
}







t_token *tokenize_input(char *input, t_env *env_list)
{
    int i = 0;
    t_token *tokens = NULL;

    while (input[i])
    {
        if (isspace(input[i]))
        {
            i++;
            continue;
        }
        i = process_token(input, i, &tokens, env_list);
        if (i == -1)
        {
            free_tokens(tokens);
            return NULL;
        }
    }
    return tokens;
}


int handle_operator(char *input, int i, t_token **tokens)
{
    char *operator_str;
    int operator_type;
    int shift;

    shift = detect_operator(input, i, &operator_str, &operator_type);
    if (shift > 0)
    {
        add_token(tokens, create_token(operator_str, operator_type, 0));
        return (i + shift);
    }
    return i;
}



int handle_regular_word(char *input, int start, t_token **tokens, t_env *env_list)
{
    // printf("DEBUG: Appel à handle_regular_characters pour un mot régulier\n");
    return handle_regular_characters(input, start, tokens, env_list);
}



int add_variable_token(char *name, char *value, t_token **tokens, int i)
{
    char *final_variable = ft_strjoin(name, "=");
    char *result = ft_strjoin(final_variable, value);

    if (!final_variable || !result)
    {
        fprintf(stderr, "minishell: memory allocation failed for final_variable or result\n");
        free(final_variable);
        free(result);
        return -1;
    }

    // printf("DEBUG: Variable finale: '%s'\n", result);

    free(final_variable);

    add_token(tokens, create_token(result, TYPE_WORD, 1));
    // printf("DEBUG: Token ajouté avec la valeur: '%s'\n", result);
    free(result);

    i++; // Avancer après la quote fermante
    // printf("DEBUG: Position après traitement: %d\n", i);

    return i;
}


int parse_variable_assignment(char *input, int i, int start, t_token **tokens)
{
    i++; // Avancer après '='
    i++; // Avancer après le premier '"'

    int value_start = i;
    // printf("DEBUG: Début de la valeur après quote à la position %d\n", value_start);

    // Parcourir la valeur jusqu'à la quote fermante
    while (input[i] && input[i] != '"')
    {
        // printf("DEBUG: Parcours de la valeur quoted, position %d, caractère: '%c'\n", i, input[i]);
        i++;
    }

    if (input[i] == '"') // Quote fermante trouvée
    {
        // printf("DEBUG: Fin de la valeur quoted à la position %d\n", i);

        char *name = ft_substr(input, start, value_start - start - 2);
        char *value = ft_substr(input, value_start, i - value_start);

        if (!name || !value)
        {
            fprintf(stderr, "minishell: memory allocation failed for name or value\n");
            free(name);
            free(value);
            return -1;
        }

        // printf("DEBUG: Nom extrait: '%s', Valeur extraite: '%s'\n", name, value);

        // Ajout du token pour la variable
        i = add_variable_token(name, value, tokens, i);

        free(name);
        free(value);

        return i;
    }
    else
    {
        fprintf(stderr, "minishell: syntax error: unclosed quote\n");
        return -1;
    }
}


int handle_word(char *input, int i, t_token **tokens, t_env *env_list)
{
    // printf("DEBUG: Début de handle_word, position initiale: %d, caractère: '%c'\n", i, input[i]);

    int start = i;

    // Parcourir les caractères jusqu'à un espace ou un '='
    while (input[i] && !isspace(input[i]) && input[i] != '=')
    {
        // printf("DEBUG: Parcours de input[%d]: '%c'\n", i, input[i]);
        i++;
    }

    // Si une assignation est détectée
    if (input[i] == '=' && input[i + 1] == '"')
    {
        // printf("DEBUG: Détection d'une assignation avec quotes à la position %d\n", i);
        return parse_variable_assignment(input, i, start, tokens);
    }
    else
    {
        // printf("DEBUG: Aucun '=' ou assignation détectée, appel à handle_regular_characters\n");
        return handle_regular_word(input, start, tokens, env_list);
    }
}





char *extract_quoted_content(char *input, int *i, char quote_type)
{
    int start = ++(*i); // Avance au caractère après l'ouverture de la quote
    while (input[*i] && input[*i] != quote_type)
        (*i)++;

    if (input[*i] != quote_type) // Quote non fermée
    {
        fprintf(stderr, "minishell: syntax error: unclosed quote\n");
        return NULL;
    }

    char *quoted_content = ft_substr(input, start, *i - start);
    (*i)++; // Avance après la quote fermante

    return quoted_content;
}


char *expand_or_copy_content(char *quoted_content, char quote_type, t_env *env_list)
{
    // printf("DEBUG: Début de expand_or_copy_content\n");
    // printf("DEBUG: quote_type: '%c', quoted_content: '%s'\n", quote_type, quoted_content);

    if (quote_type == '\'') // Quote simple, pas d'expansion
    {
        // printf("DEBUG: Quote simple détectée, aucun traitement nécessaire\n");
        char *copied_content = ft_strdup(quoted_content);
        if (!copied_content)
        {
            fprintf(stderr, "minishell: memory allocation failed for copied_content\n");
        }
        // printf("DEBUG: Contenu copié pour quote simple: '%s'\n", copied_content);
        return copied_content;
    }
    else if (quote_type == '"') // Quote double, expansion des variables
    {
        // printf("DEBUG: Quote double détectée, début de l'expansion des variables\n");
        char *expanded_content = expand_variables(quoted_content, env_list);
        if (!expanded_content)
        {
            fprintf(stderr, "minishell: memory allocation failed for expanded_content\n");
        }
        // printf("DEBUG: Contenu après expansion des variables: '%s'\n", expanded_content);
        return expanded_content;
    }

    // printf("DEBUG: Cas imprévu dans expand_or_copy_content, quote_type: '%c'\n", quote_type);
    return NULL; // Cas imprévu
}

char *safe_concat(char *final_content, char *new_content)
{
    char *temp = final_content;
    final_content = ft_strjoin(temp, new_content);
    free(temp);
    return final_content;
}

char *handle_adjacent_text(char *input, int *i, char *final_content)
{
    int start = *i;
    while (input[*i] && !isspace(input[*i]) && !is_operator(input[*i]))
        (*i)++;

    if (*i > start)
    {
        char *adjacent_text = ft_substr(input, start, *i - start);
        final_content = safe_concat(final_content, adjacent_text);
        free(adjacent_text);
    }
    return final_content;
}

int handle_variable_quotes(char *input, int i, t_token **tokens, t_env *env_list)
{
    // printf("DEBUG: Début de handle_variable_quotes, input[%d]: '%c'\n", i, input[i]);

    char *final_content = ft_strdup("");
    if (!final_content)
    {
        fprintf(stderr, "minishell: memory allocation failed for final_content\n");
        return -1;
    }
    // printf("DEBUG: initialisation de final_content avec une chaîne vide\n");

    while (input[i] == '\'' || input[i] == '"')
    {
        char quote_type = input[i];
        // printf("DEBUG: Détection de quote '%c' à la position %d\n", quote_type, i);

        char *quoted_content = extract_quoted_content(input, &i, quote_type);
        if (!quoted_content)
        {
            fprintf(stderr, "minishell: failed to extract quoted content\n");
            free(final_content);
            return -1;
        }
        // printf("DEBUG: Contenu extrait entre quotes: '%s'\n", quoted_content);
        // printf("input[i-1] = %c\n", input[i-1]);
        // printf("input[i] = %c\n", input[i]);
        char *expanded_content = expand_or_copy_content(quoted_content, quote_type, env_list);
        free(quoted_content);

        if (!expanded_content)
        {
            fprintf(stderr, "minishell: memory allocation failed for expanded_content\n");
            free(final_content);
            return -1;
        }
        // printf("DEBUG: Contenu après expansion (si nécessaire): '%s'\n", expanded_content);

        final_content = safe_concat(final_content, expanded_content);
        free(expanded_content);

        if (!final_content)
        {
            fprintf(stderr, "minishell: memory allocation failed during concatenation\n");
            return -1;
        }
        // printf("DEBUG: Contenu final actuel après concaténation: '%s'\n", final_content);
    }

    // printf("DEBUG: Fin des quotes, gestion du texte adjacent\n");
    final_content = handle_adjacent_text(input, &i, final_content);

    if (!final_content)
    {
        fprintf(stderr, "minishell: memory allocation failed during adjacent text handling\n");
        return -1;
    }
    // printf("DEBUG: Contenu après gestion du texte adjacent: '%s'\n", final_content);

    add_token(tokens, create_token(final_content, TYPE_WORD, 0));
    // printf("DEBUG: Token ajouté avec la valeur: '%s'\n", final_content);
    free(final_content);

    // printf("DEBUG: Fin de handle_variable_quotes, nouvelle position: %d\n", i);
    return i;
}






int check_and_handle_quotes(char *input, int i, t_token **tokens, t_env *env_list)
{
    if (i > 0 && (input[i] == '\'' || input[i] == '"'))
        return handle_variable_quotes(input, i, tokens, env_list);
    return -1;
}


int handle_special_variable(char *input, int i, t_token **tokens, t_env *env_list, char *dollar_sequence)
{
    char *exit_status = ft_itoa(env_list->exit_status);
    if (!exit_status)
    {
        fprintf(stderr, "minishell: memory allocation failed for exit_status\n");
        free(dollar_sequence);
        return -1;
    }

    char *token_value = ft_strdup(exit_status);
    free(exit_status);

    while (input[i + 1] && !isspace(input[i + 1]) && !is_operator(input[i + 1]))
    {
        char temp[2] = {input[++i], '\0'};
        char *new_token = ft_strjoin(token_value, temp);
        free(token_value);
        token_value = new_token;
    }

    add_token(tokens, create_token(token_value, TYPE_WORD, 0));
    free(token_value);
    free(dollar_sequence);
    return i + 1;
}

int handle_numeric_variable(char *input, int i, t_token **tokens, t_env *env_list, char *dollar_sequence)
{
    char var_name[2] = {input[i], '\0'};
    char *var_value = get_env_variable(env_list, var_name);

    char *token_value = var_value ? ft_strdup(var_value) : ft_strdup("");

    i++; // Avance après le chiffre

    while (input[i] && !isspace(input[i]) && !ft_isalnum(input[i]) && input[i] != '_')
    {
        char temp[2] = {input[i], '\0'};
        char *new_token = ft_strjoin(token_value, temp);
        free(token_value);
        token_value = new_token;
        i++;
    }

    add_token(tokens, create_token(token_value, TYPE_WORD, 0));
    free(token_value);
    free(dollar_sequence);
    return i;
}
int accumulate_dollars_sequence(char *input, int i, char **dollar_sequence, int *dollar_count)
{
    *dollar_sequence = NULL;
    *dollar_count = 0;

    int start = i;
    while (input[i] == '$')
    {
        (*dollar_count)++;
        i++;
    }

    *dollar_sequence = ft_substr(input, start, i - start);
    if (!*dollar_sequence)
        return -1; // Échec d'allocation
    return i;
}

int handle_variable_reference(char *input, int i, t_token **tokens, t_env *env_list)
{
    char *dollar_sequence = NULL;
    int dollar_count = 0;

    // Gestion des quotes
    int result = check_and_handle_quotes(input, i, tokens, env_list);
    if (result != -1)
        return result;

    // Accumulation des '$'
    int new_i = accumulate_dollars_sequence(input, i, &dollar_sequence, &dollar_count);
    if (new_i == -1)
        return -1;
    i = new_i;

    // Gestion des cas spécifiques
    if (input[i] == '?')
        return handle_special_variable(input, i, tokens, env_list, dollar_sequence);

    if (input[i] && ft_isdigit(input[i]))
        return handle_numeric_variable(input, i, tokens, env_list, dollar_sequence);

    if (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
        return handle_valid_variable(input, i, dollar_count, dollar_sequence, tokens, env_list);

    // Gestion des cas invalides
    int initial_index = i - ft_strlen(dollar_sequence) - 1;
    return handle_invalid_variable(input, initial_index, dollar_sequence, tokens);
}
