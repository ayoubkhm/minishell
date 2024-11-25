#include "parsing.h"

char *get_positional_argument(int index, t_env *env_list)
{
    // Vérifier si l'index est valide
    if (index < 0)
        return NULL;

    t_env *current = env_list;

    // Parcourir la liste des variables d'environnement
    while (current)
    {
        // Chercher la variable contenant les arguments (par convention "ARGV")
        if (current->name && ft_strcmp(current->name, "ARGV") == 0)
        {
            // Extraire les arguments positionnels
            char **argv = ft_split(current->value, ' ');
            char *result = argv[index] ? ft_strdup(argv[index]) : NULL;
            free_split(argv); // Libérer le tableau temporaire
            return result;
        }
        current = current->next;
    }

    return NULL; // Si aucun argument positionnel n'est trouvé
}



char *ft_strjoin_multi(int count, ...)
{
    va_list args;
    int total_length = 0;
    char *result = NULL;

    // Calculer la longueur totale
    va_start(args, count);
    for (int i = 0; i < count; i++)
    {
        char *str = va_arg(args, char *);
        if (str)
            total_length += ft_strlen(str);
    }
    va_end(args);

    // Allouer la mémoire pour la chaîne finale
    result = malloc(total_length + 1);
    if (!result)
        return NULL;

    // Construire la chaîne finale
    result[0] = '\0';
    va_start(args, count);
    for (int i = 0; i < count; i++)
    {
        char *str = va_arg(args, char *);
        if (str)
            ft_strcat(result, str); // Concaténer chaque chaîne
    }
    va_end(args);

    return result;
}



int handle_positional_variable(char *input, int i, t_token **tokens, t_env *env_list)
{
    int start = i;
    while (input[i] && ft_isdigit(input[i]))
        i++;
    char *pos_number = ft_substr(input, start, i - start);
    int pos_index = ft_atoi(pos_number); // Convertir en entier
    free(pos_number);
    char *pos_value = get_positional_argument(pos_index, env_list);

    char *full_value = NULL;
    if (pos_value && pos_value[0] != '\0')
    {
        full_value = ft_strdup(pos_value);
    }
    else
    {
        full_value = ft_strdup(""); // Chaîne vide si pos_value est NULL
    }
    if (full_value[0] == '\0') // Chaîne vide
    {
        free(full_value);
        free(pos_value);
        return i;
    }
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token)
    {
        free(full_value);
        free(pos_value);
        return -1; // Erreur d'allocation
    }
    new_token->value = full_value;
    add_token(tokens, new_token);
    free(pos_value);
    return i; // Retourner le nouvel index après la variable
}
