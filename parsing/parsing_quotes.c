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
            temp = handle_single_quotes(input, &i, env_list);
            // printf("DEBUG: Valeur de temp après handle_single_quotes: '%s'\n", temp ? temp : "(null)");
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








char *handle_single_quotes(char *input, int *i, t_env *env_list)
{
    // printf("DEBUG: Début de handle_single_quotes, position initiale: %d\n", *i);

    int start = *i + 1; // Début après la première quote
    int j = start;

    // printf("DEBUG: Début de la recherche de la quote fermante, position de départ: %d\n", start);

    while (input[j] && input[j] != '\'')
    {
        // printf("DEBUG: Parcours de input[%d]: '%c'\n", j, input[j]);
        j++;
    }

    if (input[j] != '\'')
    {
        fprintf(stderr, "minishell: syntax error: unclosed single quote\n");
        env_list->exit_status = 127;
        // printf("DEBUG: Quote fermante non trouvée, position actuelle: %d\n", j);
        return NULL;
    }

    // printf("DEBUG: Quote fermante trouvée à la position %d\n", j);

    // Extraire la chaîne entre les quotes simples
    char *quoted_part = ft_substr(input, start, j - start);
    if (!quoted_part)
    {
        fprintf(stderr, "minishell: memory allocation failed\n");
        // printf("DEBUG: Échec de l'allocation mémoire pour la partie extraite\n");
        return NULL;
    }

    // printf("DEBUG: Partie extraite entre les quotes simples: '%s'\n", quoted_part);

    *i = j + 1; // Mettre à jour l'index après la quote fermante
    // printf("DEBUG: Mise à jour de l'index après la quote fermante: %d\n", *i);

    return quoted_part;
}



char	*append_variable_value(char *value, char *inp, int *i, t_token **tok, t_env *env_list)
{
    // printf("DEBUG: Début de append_variable_value\n");
    // printf("DEBUG: Valeur actuelle de `value`: '%s'\n", value ? value : "(null)");
    // printf("DEBUG: Analyse de la variable à partir de input[%d]: '%s'\n", *i, inp + *i);

    char	*var_value;
    char	*temp;

    // Expansion de la variable
    var_value = handle_variable_expansion(inp, i, 1, tok, env_list);
    // printf("DEBUG: Valeur de la variable après expansion: '%s'\n", var_value ? var_value : "(null)");

    // Concaténation des chaînes
    temp = ft_strjoin(value, var_value);
    if (!temp)
    {
        fprintf(stderr, "minishell: memory allocation failed during variable concatenation\n");
        free(value);
        free(var_value);
        return (NULL);
    }

    // printf("DEBUG: Valeur après concaténation: '%s'\n", temp);

    // Libération des anciennes chaînes
    free(value);
    free(var_value);

    // printf("DEBUG: Fin de append_variable_value\n");
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


char *handle_double_quotes(char *input, int *i, t_token **tokens, t_env *env_list)
{
    // printf("DEBUG: Début de handle_double_quotes, position initiale: %d\n", *i);

    int start = *i + 1;
    char *value = ft_strdup("");
    if (!value)
    {
        fprintf(stderr, "minishell: memory allocation failed in handle_double_quotes\n");
        return NULL;
    }

    *i = start;
    while (input[*i] && input[*i] != '"')
    {
        if (input[*i] == '$')
        {
            // printf("DEBUG: Détection de '$' à la position %d\n", *i);

            if (input[*i + 1] == '?') // Cas particulier pour $?
            {
                // printf("DEBUG: Détection de '$?' à la position %d\n", *i);
                char *exit_status_str = ft_itoa(env_list->exit_status);
                if (!exit_status_str)
                {
                    fprintf(stderr, "minishell: memory allocation failed for exit status\n");
                    free(value);
                    return NULL;
                }
                value = ft_strjoin(value, exit_status_str);
                free(exit_status_str);
                (*i) += 2; // Passer le `$?`
            }
            else if (!input[*i + 1] || isspace(input[*i + 1]) || 
                     !(isalnum((unsigned char)input[*i + 1]) || input[*i + 1] == '_'))
            {
                // printf("DEBUG: '$' suivi d'un caractère invalide ou d'un espace, ajout direct de '$'\n");
                value = append_character(value, input[(*i)++]);
            }
            else
            {
                // printf("DEBUG: Expansion de la variable après '$'\n");
                value = append_variable_value(value, input, i, tokens, env_list);
            }
        }
        else
        {
            // printf("DEBUG: Ajout du caractère '%c' à la position %d\n", input[*i], *i);
            value = append_character(value, input[*i]);
            (*i)++;
        }

        if (!value)
        {
            fprintf(stderr, "minishell: memory allocation failed during string construction\n");
            return NULL;
        }
    }

    if (input[*i] != '"')
    {
        fprintf(stderr, "minishell: syntax error: unclosed double quote\n");
        env_list->exit_status = 127;
        free(value);
        return NULL;
    }

    // printf("DEBUG: Fin des double quotes, position actuelle: %d\n", *i);
    (*i)++;
    return value;
}


