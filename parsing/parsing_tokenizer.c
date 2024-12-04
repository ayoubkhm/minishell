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


int handle_regular_characters(char *input, int i, t_token **tokens, t_env *env_list)
{
    int start = i;
    while (input[i] && !isspace(input[i]) && !is_operator(input[i]) && input[i] != '\'' && input[i] != '"' && input[i] != '$')
    {
        i++;
    }

    char *prefix = NULL;
    if (start < i)
    {
        prefix = ft_substr(input, start, i - start);
    }

    if (input[i] == '$')
    {
        char *var_value;
        i = handle_variable_reference(input, i, tokens, env_list);
        var_value = (*tokens)->value;
        if (prefix && *prefix) // Ajout condition pour ignorer les prefixes vides
        {
            char *combined = ft_strjoin(prefix, var_value);
            free(prefix);
            free(var_value);
            (*tokens)->value = combined;
        }
    }
    else if (prefix && *prefix) // Ignore les tokens vides
    {
        add_token(tokens, create_token(prefix, TYPE_WORD, 1));
        free(prefix);
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


int handle_word(char *input, int i, t_token **tokens, t_env *env_list)
{
    int start = i;

    while (input[i] && !isspace(input[i]) && input[i] != '=')
    {
        i++;
    }

    if (input[i] == '=' && input[i + 1] == '"')
    {
        i++;
        i++;

        int value_start = i;
        while (input[i] && input[i] != '"')
        {
            i++;
        }

        if (input[i] == '"')
        {
            char *name = ft_substr(input, start, value_start - start - 2);
            char *value = ft_substr(input, value_start, i - value_start);

            char *final_variable = ft_strjoin(name, "=");
            char *result = ft_strjoin(final_variable, value);

            free(name);
            free(value);
            free(final_variable);

            add_token(tokens, create_token(result, TYPE_WORD, 1));
            free(result);

            i++;
            return i;
        }
        else
        {
            fprintf(stderr, "minishell: syntax error: unclosed quote\n");
            return -1;
        }
    }
    else
    {
        return handle_regular_characters(input, start, tokens, env_list);
    }
}



int handle_variable_quotes(char *input, int i, t_token **tokens, t_env *env_list)
{
    char *final_content = ft_strdup("");
    if (!final_content)
    {
        fprintf(stderr, "minishell: memory allocation failed for final_content\n");
        return -1;
    }

    while (input[i] == '\'' || input[i] == '"')
    {
        char quote_type = input[i];
        int start = ++i;

        while (input[i] && input[i] != quote_type)
            i++;

        if (input[i] != quote_type)
        {
            fprintf(stderr, "minishell: syntax error: unclosed quote\n");
            free(final_content);
            return -1;
        }
        char *quoted_content = ft_substr(input, start, i - start);
        if (!quoted_content)
        {
            fprintf(stderr, "minishell: memory allocation failed in handle_variable_quotes\n");
            free(final_content);
            return -1;
        }

        char *expanded_content = NULL;
        if (quote_type == '\'')
        {
            expanded_content = ft_strdup(quoted_content);
        }
        else if (quote_type == '"')
        {
            expanded_content = expand_variables(quoted_content, env_list);
        }
        free(quoted_content);

        if (!expanded_content)
        {
            fprintf(stderr, "minishell: memory allocation failed for expanded_content\n");
            free(final_content);
            return -1;
        }
        char *temp = final_content;
        final_content = ft_strjoin(temp, expanded_content);
        free(temp);
        free(expanded_content);

        if (!final_content)
        {
            fprintf(stderr, "minishell: memory allocation failed during concatenation\n");
            return -1;
        }

        i++;
    }
    int adjacent_start = i;

    while (input[i] && !isspace(input[i]) && !is_operator(input[i]))
        i++;

    if (i > adjacent_start)
    {
        char *adjacent_text = ft_substr(input, adjacent_start, i - adjacent_start);
        char *temp = final_content;
        final_content = ft_strjoin(temp, adjacent_text);
        free(temp);
        free(adjacent_text);
    }

    add_token(tokens, create_token(final_content, TYPE_WORD, 0));
    free(final_content);

    return i;
}







int handle_variable_reference(char *input, int i, t_token **tokens, t_env *env_list)
{
    char *dollar_sequence = NULL;
    int dollar_count = 0;

    // Vérification pour les quotes
    if (i > 0 && (input[i] == '\'' || input[i] == '"'))
    {
        return handle_variable_quotes(input, i, tokens, env_list);
    }

    // Accumulation des '$'
    int new_i = accumulate_dollars(input, i, &dollar_sequence, &dollar_count);
    if (new_i == -1)
    {
        return -1;
    }
    i = new_i;

    // Gestion des variables spéciales comme $? (code de retour)
    if (input[i] == '?')
    {
        char *exit_status = ft_itoa(g_last_exit_status); // Remplace g_last_exit_status par la variable correspondante
        add_token(tokens, create_token(exit_status, TYPE_WORD, 0));
        free(exit_status);
        free(dollar_sequence);
        return i + 1; // Passe au caractère suivant après '?'
    }

    // Gestion des variables numériques ($9, etc.)
    if (input[i] && ft_isdigit(input[i]))
    {
        char var_name[2] = {input[i], '\0'}; // Nom de variable "9"
        char *var_value = get_env_variable(env_list, var_name);

        // Initialisation du token avec la valeur de la variable ou une chaîne vide
        char *token_value = var_value ? ft_strdup(var_value) : ft_strdup("");

        i++; // Avance après le chiffre

        // Inclusion des caractères spéciaux immédiatement après
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

    // Gestion des variables alphanumériques valides
    if (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
    {
        return handle_valid_variable(input, i, dollar_count, dollar_sequence, tokens, env_list);
    }

    // Gestion des variables invalides
    int initial_index = i - ft_strlen(dollar_sequence) - 1;
    return handle_invalid_variable(input, initial_index, dollar_sequence, tokens);
}





