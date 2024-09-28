#include "parsing.h"
#include <stdlib.h>

char *handle_variable_expansion(char *input, int *i, int in_quotes, t_token **tokens)
{
    int start = *i + 1;  // Ignorer le symbole '$'
    int var_len = 0;

    // Parcourir le nom de la variable
    while (input[start + var_len] && (ft_isalnum(input[start + var_len]) || input[start + var_len] == '_'))
        var_len++;

    // Extraire le nom de la variable
    char *var_name = ft_substr(input, start, var_len);

    // Obtenir la valeur de la variable d'environnement
    char *var_value = getenv(var_name);
    free(var_name);

    // Mettre à jour l'index i
    *i = start + var_len;

    // Si on est pas dans des quotes, retourner la valeur
    if (in_quotes)
    {
        if (var_value)
            return ft_strdup(var_value);
        else
            return ft_strdup("");
    }
    else
    {
        // Si on est pas dans des quotes, ajouter le token directement
        if (var_value)
            add_token(tokens, create_token(var_value, TYPE_ENV_VAR));
        else
            add_token(tokens, create_token("", TYPE_ENV_VAR));

        return NULL;
    }
}
