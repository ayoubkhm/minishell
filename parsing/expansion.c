#include "parsing.h"

char *expand_variable(char *arg, int *i, t_env *env_list, char *result)
{
    int var_start;
    char *var_name;
    char *var_value;
    char *temp;

    *i = *i + 1;

    if (arg[*i] == '?')
    {
        var_value = ft_itoa(g_last_exit_status);

        temp = result;
        result = ft_strjoin(result, var_value);
        free(temp);
        free(var_value);

        *i = *i + 1;
        return result;
    }
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
    char *result;
    int i;

    result = strdup("");
    i = 0;

    while (arg[i])
    {
        if (arg[i] == '$')
        {
            // Vérification pour $?
            if (arg[i + 1] == '?')
            {
                char *status_str = ft_itoa(g_last_exit_status);
                char *temp = result;
                result = ft_strjoin(result, status_str);
                free(temp);
                free(status_str);
                i += 2; // Avancer après "$?"
                continue;
            }

            // Expansion classique des variables
            result = expand_variable(arg, &i, env_list, result);
        }
        else
        {
            // Ajouter le caractère au résultat
            result = append_character_main(result, arg[i++]);
        }
    }

    return result;
}
