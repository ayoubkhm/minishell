/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_chars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 03:03:27 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 22:05:51 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	handle_backslash(char *input, int i, char **current_value)
{
	char	escaped_char[2];
	char	literal_char[2];
	char	*temp;

	if (!input[i + 1])
	{
		return (-1);
	}
	escaped_char[0] = input[i + 1];
	escaped_char[1] = '\0';
	temp = *current_value;
	*current_value = ft_strjoin(*current_value, escaped_char);
	free(temp);
	i += 2;
	while (input[i] && !isspace(input[i]) && !is_operator(input[i]))
	{
		literal_char[0] = input[i];
		literal_char[1] = '\0';
		temp = *current_value;
		*current_value = ft_strjoin(*current_value, literal_char);
		free(temp);
		i++;
	}
	return (i);
}
int handle_character_iteration(char *input, int i)
{
    int in_single_quotes = 0;
    int in_double_quotes = 0;

    while (input[i])
    {
        if (in_single_quotes)
        {
            // A l'intérieur des quotes simples
            if (input[i] == '\'')
            {
                in_single_quotes = 0; // Sortie des quotes simples
                i++;
                continue;
            }
            i++;
        }
        else if (in_double_quotes)
        {
            // A l'intérieur des double quotes
            if (input[i] == '"')
            {
                in_double_quotes = 0; // Sortie des double quotes
                i++;
                continue;
            }
            i++;
        }
        else
        {
            // En dehors de toute quote
            // On s'arrête si on rencontre un espace, un opérateur, un $, ou une quote
            if (isspace((unsigned char)input[i]) || is_operator(input[i]) || input[i] == '$')
                break;
            if (input[i] == '\'')
            {
                in_single_quotes = 1;
                i++;
                continue;
            }
            if (input[i] == '"')
            {
                in_double_quotes = 1;
                i++;
                continue;
            }
            i++;
        }
    }
    return i;
}


char *extract_prefix(char *input, int start, int i)
{
    if (start < i)
    {
        char *prefix = ft_substr(input, start, i - start);
        if (!prefix)
            return NULL;
        char *cleaned = remove_quotes(prefix); // retire ' et "
        free(prefix);
        return cleaned;
    }
    return NULL;
}

void process_prefix_as_word(char *prefix, t_token **tokens)
{
    if (prefix && *prefix)
    {
        add_token(tokens, create_token(prefix, TYPE_WORD, 0));
    }
}

int handle_regular_characters(char *inp, int i, t_token **tok, t_env *env_list)
{
    t_pars_cxt cxt;

    cxt.inp = inp;
    cxt.i = i;
    cxt.pfx = NULL;
    cxt.tok = tok;
    cxt.e_l = env_list;

    // Étape 1 : Itération sur les caractères réguliers
    cxt.i = handle_character_iteration(cxt.inp, cxt.i);

    // Étape 2 : Extraction du préfixe
    cxt.pfx = extract_prefix(cxt.inp, i, cxt.i);

    // Étape 3 : Gestion des variables
    if (cxt.inp[cxt.i] == '$')
    {
        cxt.i = gere_var2(&cxt);
        if (*cxt.tok == NULL || (*cxt.tok)->value == NULL)
        {
            free(cxt.pfx);  // Libération si la variable n'est pas valide (pfx n'a pas été libéré)
            return cxt.i;
        }
        return cxt.i;
    }

    // Étape 4 : Si un préfixe existe mais n'est pas traité comme une variable
    if (cxt.pfx)
    {
        // process_prefix_as_word libère déjà cxt.pfx
        process_prefix_as_word(cxt.pfx, cxt.tok);
        free(cxt.pfx);

        cxt.pfx = NULL;
    }

    return cxt.i;
}

