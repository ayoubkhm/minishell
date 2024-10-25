#include "parsing.h"

int handle_single_quotes(char *input, int i, t_token **tokens)
{
    int start = ++i;

    // Parcours jusqu'à la fermeture de la quote simple
    while (input[i] && input[i] != '\'')
    {
        i++;
    }

    // Si la quote simple n'est pas fermée, on affihche une erreur tmtc
    if (input[i] != '\'')
    {
        fprintf(stderr, "minishell: syntax error: unclosed single quote\n");
        return i;
    }

    i++;  // Ignorer la quote fermante

    // Créer un token avec tout le contenu entre quotes simples
    char *value = ft_substr(input, start, i - start - 1);  // -1 pour exclure la quote qui ferme
    add_token(tokens, create_token(value, TYPE_QUOTED));
    free(value);

    return i;  // Retourner l'index mis à jour après la quote fermante
}



int handle_double_quotes(char *input, int i, t_token **tokens)
{
    i++;  // Ignorer la quote ouvrante
    char *value = ft_strdup("");  // Ici on initialise la chaîne vide

    while (input[i] && input[i] != '"')
    {
        if (input[i] == '$')
        {
            // Dans les quotes, on récupère la valeur pour la cat
            char *var_value = handle_variable_expansion(input, &i, 1, tokens);
            char *temp = ft_strjoin(value, var_value);
            free(value);
            free(var_value);
            value = temp;
        }
        else
        {
            // ici le caractère courant à la chaîne
            char temp_str[2] = {input[i], '\0'};
            char *temp = ft_strjoin(value, temp_str);
            free(value);
            value = temp;
            i++;
        }
    }

    // Si la quote double n'est pas fermée, on affiche une erreur
    if (input[i] != '"')
    {
        fprintf(stderr, "minishell: syntax error: unclosed double quote\n");
        free(value);
        return i;
    }

    i++;  // ignore la 2ème quote 

    // Ajouter le token avec la chaîne complète comme TYPE_QUOTED
    add_token(tokens, create_token(value, TYPE_QUOTED));
    free(value);

    return i;  // Retourner  l'index mis à jour après la quote fermante
}
