#include "parsing.h"

char *expand_variable(char *arg, int *i, t_env *env_list, char *result)
{
    int var_start;
    char *var_name;
    char *var_value;
    char *temp;

    *i = *i + 1; // Avance après '$'

    // Vérifie si le caractère suivant est valide
    if (!arg[*i] || !(isalnum((unsigned char)arg[*i]) || arg[*i] == '_'))
    {
        // Pas de nom de variable valide, on ajoute '$' au résultat
        result = append_character_main(result, '$');
        return result;
    }

    // Début de l'extraction du nom de variable
    var_start = *i;
    while (arg[*i] && (isalnum((unsigned char)arg[*i]) || arg[*i] == '_'))
    {
        *i = *i + 1;
    }
    var_name = ft_strndup(&arg[var_start], *i - var_start);
    var_value = get_env_variable(env_list, var_name);
    if (var_value)
    {
        temp = result;
        result = ft_strjoin(result, var_value);
        free(temp);
    }
    // Si la variable n'existe pas, on n'ajoute rien (comme Bash)
    free(var_name);
    return result;
}



char	*append_character_main(char *result, char c)
{
	char	temp_str[2];
	char	*temp;

	temp_str[0] = c;
	temp_str[1] = '\0';
	temp = result;
	result = ft_strjoin(result, temp_str);
	free(temp);
	return (result);
}

char *expand_variables(char *arg, t_env *env_list)
{
    // printf("DEBUG: Début de expand_variables, argument: '%s'\n", arg);

    char *result = strdup("");
    if (!result)
    {
        fprintf(stderr, "minishell: memory allocation failed for result in expand_variables\n");
        return NULL;
    }
    // printf("DEBUG: Initialisation de result avec une chaîne vide\n");

    int i = 0;

    while (arg[i])
    {
        // printf("DEBUG: Parcours de arg[%d]: '%c'\n", i, arg[i]);

        if (arg[i] == '$')
        {
            // printf("DEBUG: Détection du symbole '$' à la position %d\n", i);

            // Vérification pour $?
            if (arg[i + 1] == '?')
            {
                // printf("DEBUG: Détection de la séquence '$?' à la position %d\n", i);
                // Ajoutez ici le traitement pour $? (par exemple en utilisant env_list->exit_status)
                char *exit_status = ft_itoa(env_list->exit_status);
                if (!exit_status)
                {
                    fprintf(stderr, "minishell: memory allocation failed for exit_status\n");
                    free(result);
                    return NULL;
                }
                // printf("DEBUG: Valeur de exit_status: '%s'\n", exit_status);

                char *temp = result;
                result = ft_strjoin(temp, exit_status);
                free(temp);
                free(exit_status);
                if (!result)
                {
                    fprintf(stderr, "minishell: memory allocation failed during $? handling\n");
                    return NULL;
                }
                // printf("DEBUG: Résultat après expansion de $?: '%s'\n", result);
                i += 2; // Avance après $? (2 caractères)
            }
            else if (!arg[i + 1] || !(isalnum((unsigned char)arg[i + 1]) || arg[i + 1] == '_'))
            {
                // printf("DEBUG: Caractère suivant '$' non valide ou absent à la position %d, ajout de '$' au résultat\n", i);
                result = append_character_main(result, arg[i++]);
                if (!result)
                {
                    fprintf(stderr, "minishell: memory allocation failed during invalid $ handling\n");
                    return NULL;
                }
                // printf("DEBUG: Résultat après ajout de '$': '%s'\n", result);
            }
            else
            {
                // printf("DEBUG: Début de l'expansion classique de la variable à la position %d\n", i);
                result = expand_variable(arg, &i, env_list, result);
                if (!result)
                {
                    fprintf(stderr, "minishell: memory allocation failed during variable expansion\n");
                    return NULL;
                }
                // printf("DEBUG: Résultat après expansion de variable: '%s'\n", result);
            }
        }
        else
        {
            // printf("DEBUG: Ajout du caractère '%c' au résultat à la position %d\n", arg[i], i);
            result = append_character_main(result, arg[i++]);
            if (!result)
            {
                fprintf(stderr, "minishell: memory allocation failed during character append\n");
                return NULL;
            }
            // printf("DEBUG: Résultat après ajout du caractère: '%s'\n", result);
        }
    }

    // printf("DEBUG: Fin de expand_variables, résultat final: '%s'\n", result);
    return result;
}


