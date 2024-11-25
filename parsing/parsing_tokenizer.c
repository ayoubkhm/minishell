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
    if (!tokens) // Si la liste est vide
        return NULL;

    while (tokens->next) // Parcourt jusqu'à la fin de la liste
        tokens = tokens->next;

    return tokens; // Retourne le dernier token
}


void remove_last_token(t_token **tokens)
{
    if (!tokens || !*tokens)
        return;

    t_token *current = *tokens;

    // Si la liste ne contient qu'un seul token
    if (!current->next)
    {
        free(current->value);  // Libère la valeur du token
        free(current);         // Libère la structure du token
        printf("Le Free a eu lieu là\n");
        *tokens = NULL;        // Met à jour la tête de la liste
        return;
    }

    // Parcourt la liste pour trouver l'avant-dernier token
    while (current->next && current->next->next)
    {
        current = current->next;
    }

    // Libère le dernier token
    free(current->next->value);
    free(current->next);
    printf("Le Free a eu lieu ici\n");
    current->next = NULL; // Met à jour le pointeur du dernier élément
}


int process_token(char *input, int i, t_token **tokens, t_env *env_list)
{
    char *current_value = NULL; // Initialiser à NULL pour éviter les allocations inutiles

    if (input[i] == '$')
    {
        return handle_variable_reference(input, i, tokens, env_list);
    }
    else if (is_operator(input[i]))
    {
        return handle_operator(input, i, tokens);
    }
    else if (input[i] == '\'' || input[i] == '"')
    {
        return handle_quotes_in_word(input, i, tokens, input[i], input[i] == '"');
    }
    else if (input[i] == '[')
    {
        return handle_brackets(input, i, tokens, env_list);
    }
    else if (input[i] == '\\') // Gérer le backslash
    {
        current_value = ft_strdup(""); // Allouer seulement ici si nécessaire
        if (!current_value)
            return -1;

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
    else
    {
        return handle_word(input, i, tokens, env_list);
    }
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

int handle_quotes_in_word(char *input, int i, t_token **tokens, char quote_char, int expand)
{
    int start;
    char *value;

    i++;
    start = i;

    while (input[i] && input[i] != quote_char)
    {
        i++;
    }

    if (input[i] != quote_char)
    {
        fprintf(stderr, "minishell: syntax error: unclosed %c quote\n", quote_char);
        return -1;
    }

    if (i == start)
    {
        return (i + 1);
    }

    value = ft_substr(input, start, i - start);
    add_token(tokens, create_token(value, TYPE_WORD, expand));
    free(value);

    return (i + 1); // Passe la quote fermante
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





int handle_variable_reference(char *input, int i, t_token **tokens, t_env *env_list)
{
    char *dollar_sequence = NULL;
    int dollar_count = 0;

    // Vérifier si le $ est échappé par un backslash
    if (i > 0 && input[i - 1] == '\\')
    {
        // Fusionner le $ avec les caractères suivants littéraux
        int start = i;
        while (input[i] && !isspace(input[i]) && !is_operator(input[i]))
            i++;

        char *escaped_value = ft_substr(input, start, i - start);
        add_token(tokens, create_token(escaped_value, TYPE_WORD, 0));
        free(escaped_value);

        return i;
    }

    // Accumuler les '$' successifs
    int new_i = accumulate_dollars(input, i, &dollar_sequence, &dollar_count);
    if (new_i == -1)
        return -1;

    i = new_i;

    // Vérifier si c'est une variable spéciale (comme $? ou $$)
    int result = handle_special_variable(input, i, dollar_count, dollar_sequence, tokens);
    if (result != -2) // Si un cas spécial est géré, on retourne
        return result;

    // Gestion des variables numériques (ex. $1, $9)
    if (input[i] && ft_isdigit(input[i]))
    {
        return handle_positional_variable(input, i, tokens, env_list);
    }
    // Gestion des variables alphanumériques classiques (ex. $HOME)
    else if (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
    {
        return handle_valid_variable(input, i, dollar_count, dollar_sequence, tokens, env_list);
    }
    else
    {
        // Si ce n'est ni un cas spécial ni une variable valide, traiter comme littéral
        return handle_invalid_variable(input, i, dollar_sequence, tokens);
    }
}
