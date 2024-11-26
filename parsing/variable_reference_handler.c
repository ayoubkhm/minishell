#include "parsing.h"

int accumulate_dollars(char *input, int i, char **dollar_sequence, int *dollar_count)
{

    *dollar_sequence = ft_strdup("");
    if (!*dollar_sequence)
    {
        fprintf(stderr, "[ERROR] Failed to allocate memory for dollar_sequence\n");
        return -1;
    }

    *dollar_count = 0;

    while (input[i] == '$')
    {
        (*dollar_count)++;
        char *temp = *dollar_sequence;
        *dollar_sequence = ft_strjoin(*dollar_sequence, "$");
        free(temp);

        if (!*dollar_sequence)
        {
            fprintf(stderr, "[ERROR] Failed to allocate memory for dollar_sequence during concatenation\n");
            return -1;
        }
        i++;
    }
    return i;
}



int handle_special_variable(char *input, int i, int dollar_count, char *dollar_sequence, t_token **tokens)
{
    char *var_value = NULL;
    if (input[i] == '?')
    {
        var_value = ft_itoa(g_last_exit_status);
        if (!var_value)
        {
            free(dollar_sequence);
            return -1;
        }
        char *temp = (dollar_count > 1) ? ft_substr(dollar_sequence, 0, dollar_count - 1) : ft_strdup("");
        char *combined_value = ft_strjoin(temp, var_value);
        free(temp);
        free(var_value);
        int j = i + 1;
        while (input[j] && !isspace(input[j]) && !is_operator(input[j]) && !ft_isalnum(input[j]))
            j++;

        if (j > i + 1)
        {
            char *special_chars = ft_substr(input, i + 1, j - (i + 1));
            temp = combined_value;
            combined_value = ft_strjoin(combined_value, special_chars);
            free(temp);
            free(special_chars);
        }

        add_token(tokens, create_token(combined_value, TYPE_WORD, 1));
        free(combined_value);
        free(dollar_sequence);

        return j;
    }
    else if (input[i] == '0')
    {
        var_value = ft_itoa(g_last_exit_status);
        if (!var_value)
        {
            free(dollar_sequence);
            return -1;
        }
        char *temp = ft_strjoin(dollar_sequence, var_value);
        free(dollar_sequence);
        free(var_value);
        if (!temp)
            return -1;
        add_token(tokens, create_token(temp, TYPE_WORD, 1));
        free(temp);

        return i + 1;
    }
    return -2;
}

char *extract_variable_value(char *input, int *i, t_env *env_list)
{
    char *var_name = NULL;
    char *var_value = NULL;
    char *var_value_copy = NULL;

    int var_start = *i;
    while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
    {
        (*i)++;
    }

    var_name = ft_substr(input, var_start, *i - var_start);
    if (!var_name)
        return NULL;

    var_value = get_env_variable(env_list, var_name);
    free(var_name);

    if (!var_value)
    {
        var_value_copy = ft_strdup("");
    }
    else
    {
        var_value_copy = ft_strdup(var_value);
    }

    if (!var_value_copy)
    {
        fprintf(stderr, "[ERROR] Allocation failed for var_value_copy\n");
        return NULL;
    }

    return var_value_copy;
}


char *build_combined_value(int dollar_count, char *dollar_sequence, char *var_value)
{
    char *temp = (dollar_count > 1) ? ft_substr(dollar_sequence, 0, dollar_count - 1) : ft_strdup("");
    if (!temp)
        return NULL;

    char *combined_value = ft_strjoin(temp, var_value);
    free(temp);

    return combined_value;
}


int append_special_chars(char *input, int i, char **combined_value)
{
    int j = i;
    while (input[j] && !isspace(input[j]) && !is_operator(input[j]) && !ft_isalnum(input[j]))
        j++;

    if (j > i)
    {
        char *special_chars = ft_substr(input, i, j - i);
        if (!special_chars)
            return i;

        char *temp = *combined_value;
        *combined_value = ft_strjoin(*combined_value, special_chars);
        free(temp);
        free(special_chars);

        return j;
    }

    return i;
}

int handle_valid_variable(char *input, int i, int dollar_count, char *dollar_sequence, t_token **tokens, t_env *env_list)
{
    char *var_value = extract_variable_value(input, &i, env_list);
    if (!var_value)
        return i;

    char *combined_value = build_combined_value(dollar_count, dollar_sequence, var_value);
    free(var_value);
    if (!combined_value)
        return i;

    i = append_special_chars(input, i, &combined_value);

    add_token(tokens, create_token(combined_value, TYPE_WORD, 1));
    free(combined_value);
    free(dollar_sequence);

    return i;
}






int handle_invalid_variable(char *input, int i, char *dollar_sequence, t_token **tokens)
{
    int start = i - ft_strlen(dollar_sequence);
    int j = i;

    while (input[j] && !isspace(input[j]) && !is_operator(input[j]))
    {
        j++;
    }
    char *literal_value = ft_substr(input, start, j - start);
    if (!literal_value)
    {
        fprintf(stderr, "[ERROR] Failed to allocate memory for literal_value\n");
        free(dollar_sequence);
        return -1;
    }
    add_token(tokens, create_token(literal_value, TYPE_WORD, 1));
    free(literal_value);
    free(dollar_sequence);
    return j;
}
