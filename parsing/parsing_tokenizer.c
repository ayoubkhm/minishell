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
        if (prefix)
        {
            char *combined = ft_strjoin(prefix, var_value);
            free(prefix);
            free(var_value);
            (*tokens)->value = combined;
        }
    }
    else if (prefix)
    {
        add_token(tokens, create_token(prefix, TYPE_WORD, 1));
        free(prefix);
    }

    return i;
}

int process_token(char *input, int i, t_token **tokens, t_env *env_list)
{
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
    value = ft_substr(input, start, i - start);
    add_token(tokens, create_token(value, TYPE_WORD, expand));
    free(value);
    i++;
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

int handle_variable_reference(char *input, int i, t_token **tokens, t_env *env_list)
{
    char *var_name;
    char *var_value;

    i++;
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

    if (ft_strchr(var_value, ' '))
    {
        t_token *expanded_tokens = tokenize_input(var_value, env_list);
        t_token *current = expanded_tokens;
        while (current)
        {
            add_token(tokens, create_token(current->value, TYPE_WORD, 1));
            current = current->next;
        }
        free_tokens(expanded_tokens);
    }
    else
    {
        add_token(tokens, create_token(var_value, TYPE_WORD, 1));
    }

    free(var_name);
    free(var_value);

    return (i);
}
