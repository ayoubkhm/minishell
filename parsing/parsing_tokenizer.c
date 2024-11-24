#include "parsing.h"

/**
 * Gère les guillemets simples et doubles.
 */
int handle_quotes(char *input, int i, t_token **tokens, t_env *env_list)
{
    char quote_char = input[i];
    int start = ++i; // Avancer après le guillemet ouvrant
    char *value;

    while (input[i] && input[i] != quote_char)
        i++;

    if (!input[i])
    {
        fprintf(stderr, "minishell: syntax error: unclosed quote\n");
        return -1;
    }

    value = ft_strndup(&input[start], i - start);

    if (quote_char == '"')
        value = expand_variables(value, env_list);

    add_token(tokens, create_token(value, TYPE_WORD, quote_char == '"'));
    free(value);

    return i + 1;
}

/**
 * Gère les caractères échappés avec un backslash.
 */
int handle_backslash(char *input, int i, char **current_value)
{
    if (!input[i + 1])
    {
        fprintf(stderr, "minishell: syntax error: backslash at end of input\n");
        return -1;
    }

    char escaped_char[2] = {input[i + 1], '\0'};
    char *temp = *current_value;
    *current_value = ft_strjoin(*current_value, escaped_char);
    free(temp);

    return i + 2;
}


/**
 * Gère une séquence de caractères normaux jusqu'à un opérateur ou espace.
 */
int handle_regular_characters(char *input, int i, t_token **tokens)
{
    char *current_value = ft_strdup("");
    if (!current_value)
        return -1;

    while (input[i] && !isspace(input[i]) && !is_operator(input[i]))
    {
        if (input[i] == '\\')
        {
            i = handle_backslash(input, i, &current_value);
            if (i == -1)
            {
                free(current_value);
                return -1;
            }
        }
        else if (input[i] == '$')
        {
            // Handle variable expansion if needed
            // For escaped $, it won't reach here
            break;
        }
        else
        {
            char temp_str[2] = {input[i], '\0'};
            char *temp = current_value;
            current_value = ft_strjoin(current_value, temp_str);
            free(temp);
            i++;
        }
    }

    if (*current_value)
        add_token(tokens, create_token(current_value, TYPE_WORD, 0));
    free(current_value);

    return i;
}


/**
 * Gère les opérateurs comme '|', '>' ou '<'.
 */
int handle_operator(char *input, int i, t_token **tokens)
{
    char *operator_str;
    int operator_type;
    int shift = detect_operator(input, i, &operator_str, &operator_type);

    if (shift > 0)
    {
        add_token(tokens, create_token(operator_str, operator_type, 0));
        return i + shift;
    }

    return i;
}

/**
 * Gère les variables référencées par `$`.
 */
int handle_variable_reference(char *input, int i, t_token **tokens, t_env *env_list)
{
    char *var_name;
    char *var_value;

    i++; // Avance après `$`
    if (input[i] == '?')
    {
        var_value = ft_itoa(g_last_exit_status);
        add_token(tokens, create_token(var_value, TYPE_WORD, 1));
        free(var_value);
        return i + 1;
    }

    if (!input[i] || (!ft_isalnum(input[i]) && input[i] != '_'))
    {
        add_token(tokens, create_token("$", TYPE_WORD, 0));
        return i;
    }

    int var_start = i;
    while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
        i++;

    var_name = ft_substr(input, var_start, i - var_start);
    var_value = get_env_variable(env_list, var_name);

    if (!var_value)
        var_value = ft_strdup("");

    add_token(tokens, create_token(var_value, TYPE_WORD, 1));
    free(var_name);
    free(var_value);

    return i;
}

/**
 * Parcourt l'entrée et génère les tokens.
 */
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
        else if (input[i] == '\'' || input[i] == '"')
        {
            i = handle_quotes(input, i, &tokens, env_list);
        }
        else if (is_operator(input[i]))
        {
            i = handle_operator(input, i, &tokens);
        }
        else
        {
            i = handle_regular_characters(input, i, &tokens);
        }

        if (i == -1)
        {
            free_tokens(tokens);
            return NULL;
        }
    }

    return tokens;
}


/**
 * Gère les guillemets dans les mots.
 */
int handle_quotes_in_word(char *input, int i, t_token **tokens, char quote_char, int expand)
{
    int start = ++i;

    while (input[i] && input[i] != quote_char)
        i++;

    if (!input[i])
    {
        fprintf(stderr, "minishell: syntax error: unclosed quote\n");
        return -1;
    }

    char *value = ft_substr(input, start, i - start);
    add_token(tokens, create_token(value, TYPE_WORD, expand));
    free(value);

    return i + 1;
}
