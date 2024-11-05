#include "parsing.h"

int	handle_single_quotes(char *input, int i, t_token **tokens)
{
	int		start;
	char	*value;

	start = i + 1; // Commence après la quote ouvrante
	while (input[i] && input[i] != '\'')
	{
		i++;
	}
	if (input[i] != '\'')
	{
		// Si la quote fermante n'est pas trouvée, signale une erreur
		fprintf(stderr, "minishell: syntax error: unclosed single quote\n");
		return i; // Retourne sans créer de token
	}

	// Extrait le contenu entre les quotes simples
	value = ft_substr(input, start, i - start);

	// Ajoute le token tel quel sans expansion et sans quote fermante
	add_token(tokens, create_token(value, TYPE_QUOTED, 0)); // expand = 0 pour single quotes
	free(value);

	// Retourne la position juste après la quote fermante
	return (i + 1);
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

int	handle_double_quotes(char *input, int i, t_token **tokens, t_env *env_list)
{
	char	*value;

	// Saute la quote ouvrante
	i = i + 1;
	value = build_double_quoted_string(input, &i, tokens, env_list);

	// Vérifie si une quote fermante est manquante
	if (!value) {
		fprintf(stderr, "Error: build_double_quoted_string returned NULL\n");
		return i;
	}

	add_token(tokens, create_token(value, TYPE_QUOTED, 1)); // expand = 1 pour double quotes
	free(value);
	return i;
}
