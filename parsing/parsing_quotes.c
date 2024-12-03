#include "parsing.h"

int parse_input(char *input, t_token **tokens, t_env *env_list)
{
    int i = 0;

    while (input[i])
    {
        if (isspace(input[i]))
        {
            i++;
            continue;
        }

        // Analyser un mot
        int new_i = parse_word(input, i, tokens, env_list);
        if (new_i == -1)
        {
            // Gestion de l'erreur
            return -1;
        }

        i = new_i;
    }
    return 0;
}


int parse_word(char *input, int i, t_token **tokens, t_env *env_list)
{
    char *value = ft_strdup(""); // Accumule le contenu du mot
    char *temp = NULL;

    if (!value)
    {
        fprintf(stderr, "minishell: memory allocation failed\n");
        return -1;
    }
    while (input[i] && !isspace(input[i]))
    {
        if (input[i] == '\'') // Si c'est une quote simple
        {
            temp = handle_single_quotes(input, &i);
        }
        else if (input[i] == '"') // Si c'est une quote double
        {
            temp = handle_double_quotes(input, &i, tokens, env_list);
        }
        else // Si c'est un caractère normal
        {
            temp = ft_substr(input, i, 1); // Extraire le caractère
            i++;
        }

        if (!temp)
        {
            free(value);
            return -1;
        }

        // Concaténer la partie extraite avec la valeur accumulée
        char *new_value = ft_strjoin(value, temp);
        free(value);
        free(temp);

        if (!new_value)
        {
            fprintf(stderr, "minishell: memory allocation failed\n");
            return -1;
        }

        value = new_value;
    }

    add_token(tokens, create_token(value, TYPE_WORD, 1));
    free(value);
    return i;
}








char *handle_single_quotes(char *input, int *i)
{
    int start = *i + 1;
    int j = start;

    while (input[j] && input[j] != '\'')
    {
        j++;
    }

    if (input[j] != '\'')
    {
        fprintf(stderr, "minishell: syntax error: unclosed single quote\n");
        return NULL;
    }

    // Extraire la chaîne entre les quotes simples
    char *quoted_part = ft_substr(input, start, j - start);
    if (!quoted_part)
    {
        fprintf(stderr, "minishell: memory allocation failed\n");
        return NULL;
    }

    *i = j + 1; // Mettre à jour l'index après la quote fermante
    return quoted_part;
}


char	*append_variable_value(char *value, char *inp, int *i, t_token **tok, t_env *env_list)
{
	char	*var_value;
	char	*temp;

	var_value = handle_variable_expansion(inp, i, 1, tok, env_list);
	temp = ft_strjoin(value, var_value);
	free(value);
	free(var_value);
	return (temp);
}

char	*append_character(char *value, char c)
{
	char	temp_str[2];
	char	*temp;

	temp_str[0] = c;
	temp_str[1] = '\0';
	temp = ft_strjoin(value, temp_str);
	free(value);
	return (temp);
}

char	*build_double_quoted_string(char *input, int *i, t_token **tokens, t_env *env_list)
{
    char	*value;
    value = ft_strdup("");
    while (input[*i] && input[*i] != '"')
    {
        if (input[*i] == '$')
            value = append_variable_value(value, input, i, tokens, env_list);
        else
            value = append_character(value, input[(*i)++]);
    }
    if (input[*i] == '"')
        (*i)++;
    return (value);
}

char *handle_double_quotes(char *input, int *i, t_token **tokens, t_env *env_list)
{
    int start = *i + 1;
    char *value = ft_strdup("");

    *i = start;
    while (input[*i] && input[*i] != '"')
    {
        if (input[*i] == '$')
        {
            value = append_variable_value(value, input, i, tokens, env_list);
        }
        else
        {
            value = append_character(value, input[*i]);
            (*i)++;
        }
    }

    if (input[*i] != '"')
    {
        fprintf(stderr, "minishell: syntax error: unclosed double quote\n");
        free(value);
        return NULL;
    }

    (*i)++;
    return value;
}
