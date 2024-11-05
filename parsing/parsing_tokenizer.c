#include "parsing.h"

int handle_regular_characters(char *input, int i, t_token **tokens, t_env *env_list)
{
    int start = i;
    while (input[i] && !isspace(input[i]) && !is_operator(input[i]) && input[i] != '\'' && input[i] != '"' && input[i] != '$')
    {
        i++;
    }
    if (input[i] == '$')
    {
        if (start < i)
        {
            char *value = ft_substr(input, start, i - start);
            add_token(tokens, create_token(value, TYPE_WORD, 1));
            free(value);
        }
        i = handle_variable_reference(input, i, tokens, env_list);
    }
    else
    {
        char *value = ft_substr(input, start, i - start);
        add_token(tokens, create_token(value, TYPE_WORD, 1));
        free(value);
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
    else
    {
        return (handle_word(input, i, tokens, env_list));
    }
}

t_token *tokenize_input(char *input, t_env *env_list)
{
    int i;
    t_token *tokens;

    i = 0;
    tokens = NULL;
    while (input[i])
    {
        if (isspace(input[i]))
        {
            i++;
            continue;
        }
        i = process_token(input, i, &tokens, env_list);
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

    i++; // Passe la quote ouvrante
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
    i++; // Passe la quote fermante
    return i;
}

int handle_word(char *input, int i, t_token **tokens, t_env *env_list)
{
    while (input[i] && !isspace(input[i]) && !is_operator(input[i]))
    {
        if (input[i] == '\'')
        {
            i = handle_quotes_in_word(input, i, tokens, '\'', 0);
        }
        else if (input[i] == '"')
        {
            i = handle_quotes_in_word(input, i, tokens, '"', 1);
        }
        else
        {
            i = handle_regular_characters(input, i, tokens, env_list);
        }

        if (i == -1)
            return -1;
    }
    return i;
}
