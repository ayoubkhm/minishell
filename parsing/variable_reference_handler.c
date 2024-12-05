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
    char *var_value;
    char **split_values;
    char *token_value;
    int j;

    // printf("[DEBUG] Entering handle_valid_variable at index %d, char: '%c'\n", i, input[i]);

    // Extraction de la valeur de la variable
    var_value = extract_variable_value(input, &i, env_list);
    if (!var_value || var_value[0] == '\0') // Cas où la variable n'existe pas
    {
        // printf("[DEBUG] Variable not found or empty\n");
        free(var_value);
        free(dollar_sequence);
        return i;
    }

    // printf("[DEBUG] Extracted variable value: '%s'\n", var_value);

    // Scinder la valeur en parties
    split_values = ft_split(var_value, ' ');
    free(var_value);
    if (!split_values)
    {
        // printf("[DEBUG] Failed to split variable value\n");
        free(dollar_sequence);
        return i;
    }

    // printf("[DEBUG] Split variable value into parts:\n");
    j = 0;
    while (split_values[j])
    {
        // printf("  [DEBUG] Part %d: '%s'\n", j, split_values[j]);
        j++;
    }

    j = 0;
    while (split_values[j])
    {
        // Construction de la valeur du token
        if (j == 0)
            token_value = build_combined_value(dollar_count, dollar_sequence, split_values[j]);
        else
            token_value = ft_strdup(split_values[j]);

        if (!token_value)
        {
            // printf("[DEBUG] Failed to build token value for part %d\n", j);
            j++;
            continue;
        }

        // Vérifier si des caractères immédiatement après la variable doivent être inclus
        if (j == 0 && input[i] == '"')
        {
            i++; // Passer le guillemet ouvrant
            // printf("[DEBUG] Detected double quotes after variable\n");

            // Ajouter les caractères jusqu'à la fermeture des guillemets
            while (input[i] && input[i] != '"')
            {
                char temp[2] = {input[i], '\0'};
                char *new_token_value = ft_strjoin(token_value, temp);
                free(token_value);
                token_value = new_token_value;
                i++;
            }

            if (input[i] == '"')
            {
                // printf("[DEBUG] Closing double quotes found\n");
                i++; // Passer le guillemet fermant
            }
            else
            {
                fprintf(stderr, "minishell: syntax error: unclosed double quotes\n");
                free(token_value);
                free(dollar_sequence);
                return -1; // Erreur de syntaxe
            }
        }

        // printf("[DEBUG] Adding token: '%s'\n", token_value);
        add_token(tokens, create_token(token_value, TYPE_WORD, 1));
        free(token_value);

        // Libérer dollar_sequence après le premier token
        if (j == 0)
        {
            free(dollar_sequence);
            dollar_sequence = NULL;
        }

        j++;
    }

    // Libérer la mémoire des valeurs scindées
    // printf("[DEBUG] Freeing split values\n");
    j = 0;
    while (split_values[j])
    {
        free(split_values[j]);
        j++;
    }
    free(split_values);

    // printf("[DEBUG] Exiting handle_valid_variable at index %d\n", i);
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
