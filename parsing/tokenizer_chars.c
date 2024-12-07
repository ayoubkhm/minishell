/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_chars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 03:03:27 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 18:17:41 by akhamass         ###   ########.fr       */
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

int	handle_character_iteration(char *input, int i)
{
	while (input[i] && !isspace((unsigned char)input[i])
		&& !is_operator(input[i])
		&& input[i] != '\'' && input[i] != '"' && input[i] != '$')
		i++;
	return (i);
}

char	*extract_prefix(char *input, int start, int i)
{
	if (start < i)
		return (ft_substr(input, start, i - start));
	return (NULL);
}

void	process_prefix_as_word(char *prefix, t_token **tokens)
{
	if (prefix && *prefix)
	{
		add_token(tokens, create_token(prefix, TYPE_WORD, 1));
		free(prefix);
	}
}

int	handle_regular_characters(char *inp, int i, t_token **tok, t_env *env_list)
{
	t_pars_cxt	cxt;

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

		// Si la variable est invalide
		if (*cxt.tok == NULL || (*cxt.tok)->value == NULL)
		{
			fprintf(stderr, "minishell: command not found\n");
			free(cxt.pfx);
			return (cxt.i);
		}

		// Si gere_var2 a combiné le préfixe, on sort directement
		return (cxt.i);
	}

	// Étape 4 : Traitement du préfixe comme mot s'il existe
	process_prefix_as_word(cxt.pfx, cxt.tok);

	cxt.pfx = NULL;
	return (cxt.i);
}

