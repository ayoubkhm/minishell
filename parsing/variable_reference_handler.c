#include "parsing.h"

int accumulate_dollars(char *input, int i, char **dollar_sequence, int *dollar_count)
{
    *dollar_sequence = ft_strdup("");
    if (!*dollar_sequence)
        return -1;

    *dollar_count = 0;

    while (input[i] == '$')
    {
        (*dollar_count)++;
        char *temp = *dollar_sequence;
        *dollar_sequence = ft_strjoin(*dollar_sequence, "$");
        free(temp);
        i++;
    }

    return i;
}

int handle_special_variable(char *input, int i, int dollar_count, char *dollar_sequence, t_token **tokens)
{
    char *var_value = NULL;
    if (input[i] == '?')
    {

        // Gestion de $?
        var_value = ft_itoa(g_last_exit_status);
        if (!var_value)
        {
            free(dollar_sequence);
            return -1;
        }

        // Concatène les $ littéraux avec la valeur de $?
        char *temp = (dollar_count > 1) ? ft_substr(dollar_sequence, 0, dollar_count - 1) : ft_strdup("");
        char *combined_value = ft_strjoin(temp, var_value);
        free(temp);
        free(var_value);

        // Gestion des caractères spéciaux qui suivent
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

        // Gestion spéciale de $0 pour refléter g_last_exit_status
        var_value = ft_itoa(g_last_exit_status);
        if (!var_value)
        {
            free(dollar_sequence);
            return -1;
        }

        // Concatène les $ accumulés avec la valeur de g_last_exit_status
        char *temp = ft_strjoin(dollar_sequence, var_value);
        free(dollar_sequence);
        free(var_value);

        if (!temp)
            return -1;

        // Ajoute le token au tokenizer
        add_token(tokens, create_token(temp, TYPE_WORD, 1));
        free(temp);

        return i + 1; // Avance après le '0'
    }

    // Si ce n'est pas un cas spécial, retourner -2 pour indiquer qu'il faut continuer
    return -2;
}

int handle_valid_variable(char *input, int i, int dollar_count, char *dollar_sequence, t_token **tokens, t_env *env_list)
{
    char *var_name = NULL;
    char *var_value = NULL;

    int var_start = i;
    while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
    {
        i++;
    }

    var_name = ft_substr(input, var_start, i - var_start);
    var_value = get_env_variable(env_list, var_name);

    if (!var_value)
        var_value = ft_strdup(""); // Si la variable n'existe pas, expansion en chaîne vide

    // Concatène les $ littéraux avec la valeur de la variable
    char *temp = (dollar_count > 1) ? ft_substr(dollar_sequence, 0, dollar_count - 1) : ft_strdup("");
    char *combined_value = ft_strjoin(temp, var_value);
    free(temp);
    free(var_value);

    // Gestion des caractères spéciaux qui suivent
    int j = i;
    while (input[j] && !isspace(input[j]) && !is_operator(input[j]) && !ft_isalnum(input[j]))
        j++;

    if (j > i)
    {
        char *special_chars = ft_substr(input, i, j - i);
        temp = combined_value;
        combined_value = ft_strjoin(combined_value, special_chars);
        free(temp);
        free(special_chars);
        i = j;
    }

    add_token(tokens, create_token(combined_value, TYPE_WORD, 1));
    free(combined_value);
    free(var_name);
    free(dollar_sequence);

    return i;
}

int handle_invalid_variable(char *input, int i, char *dollar_sequence, t_token **tokens)
{
    // Pas de variable valide après les $, les $ sont des littéraux
    int j = i;
    while (input[j] && !isspace(input[j]) && !is_operator(input[j]) && !ft_isalnum(input[j]))
    {
        char temp_char[2] = {input[j], '\0'};
        char *temp = dollar_sequence;
        dollar_sequence = ft_strjoin(dollar_sequence, temp_char);
        free(temp);
        j++;
    }

    add_token(tokens, create_token(dollar_sequence, TYPE_WORD, 1));
    free(dollar_sequence);
    return j;
}
