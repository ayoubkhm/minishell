#include "parsing.h"

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
    current->next = NULL; // Met à jour le pointeur du dernier élément
}


int process_token(char *input, int i, t_token **tokens, t_env *env_list)
{
    int initial_token_count = count_tokens(*tokens); // Compte initial des tokens

    if (input[i] == '$')
    {
        return (handle_variable_reference(input, i, tokens, env_list));
    }
    else if (is_operator(input[i]))
    {
        return (handle_operator(input, i, tokens));
    }
    else if (input[i] == '\'' || input[i] == '"')
    {
        return (handle_quotes_in_word(input, i, tokens, input[i], input[i] == '"'));
    }
    else
    {
        return (handle_word(input, i, tokens, env_list));
    }

    if (count_tokens(*tokens) > initial_token_count)
    {
        t_token *last_token = get_last_token(*tokens);
        if (last_token && last_token->value && last_token->value[0] == '\0')
        {
            remove_last_token(tokens);
        }
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
    char *var_name;
    char *var_value;

    i++; // Avance après le symbole '$'
    if (input[i] == '?')
    {
        // Gère la variable spéciale $?
        var_value = ft_itoa(g_last_exit_status);
        add_token(tokens, create_token(var_value, TYPE_WORD, 1));
        free(var_value);
        i++; // Avance après '?'
        return i;
    }
    else if (!input[i] || (!ft_isalnum(input[i]) && input[i] != '_'))
    {
        // Pas de nom de variable valide après '$', on traite '$' comme un caractère littéral
        add_token(tokens, create_token("$", TYPE_WORD, 1)); // expand peut être 1 ou 0 selon ta gestion
        return i; // Retourne la position actuelle
    }
    else
    {
        int var_start = i;
        while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
        {
            i++;
        }
        var_name = ft_substr(input, var_start, i - var_start);
        var_value = get_env_variable(env_list, var_name);
        if (!var_value)
        {
            var_value = ft_strdup("");
        }
        // Ajoute la valeur de la variable aux tokens
        add_token(tokens, create_token(var_value, TYPE_WORD, 1));

        free(var_name);
        free(var_value);

        return i;
    }
}

