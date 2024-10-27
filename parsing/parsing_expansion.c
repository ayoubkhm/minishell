#include "parsing.h"
#include <stdlib.h>

char *handle_variable_expansion(char *input, int *i, int in_quotes, t_token **tokens)
{
    int start = *i + 1;
    int var_len = 0;
    while (input[start + var_len] && (ft_isalnum(input[start + var_len]) || input[start + var_len] == '_'))
        var_len++;
    char *var_name = ft_substr(input, start, var_len);
    char *var_value = getenv(var_name);
    free(var_name);
    *i = start + var_len;
    if (in_quotes)
    {
        if (var_value)
            return ft_strdup(var_value);
        else
            return ft_strdup("");
    }
    else
    {
        if (var_value)
            add_token(tokens, create_token(var_value, TYPE_ENV_VAR));
        else
            add_token(tokens, create_token("", TYPE_ENV_VAR));
        return NULL;
    }
}

int handle_variable_reference(char *input, int i, t_token **tokens)
{
    int start = i;
    i++;
    while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
        i++;
    char *var_ref = ft_substr(input, start, i - start);
    add_token(tokens, create_token(var_ref, TYPE_ENV_VAR));
    free(var_ref);

    return i;
}