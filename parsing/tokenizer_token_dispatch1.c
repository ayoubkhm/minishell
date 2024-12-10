/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_token_dispatch1.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 03:02:20 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/10 02:20:57 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	handle_variable_token(char *input, int i, t_token **tokens, t_env *env_list)
{
	if (input[i] == '$')
	{
		return (gere_var_ref(input, i + 1, tokens, env_list));
	}
	if ((input[i] == '\'' || input[i] == '"') && input[i + 1] == '$')
	{
		return (gere_var_ref(input, i, tokens, env_list));
	}
	return (-1);
}

int	handle_operator_token(char *input, int i, t_token **tokens)
{
	if (is_operator(input[i]))
	{
		return (handle_operator(input, i, tokens));
	}
	return (-1);
}

int	handle_quotes_token(char *input, int i, t_token **tokens, t_env *env_list)
{
	int	new_i;

	if (input[i] == '\'' || input[i] == '"')
	{
		new_i = parse_word(input, i, tokens, env_list);
		if (new_i == -1)
			return (-1);
		return (new_i);
	}
	return (-1);
}

int	handle_brackets_token(char *input, int i, t_token **tokens, t_env *env_list)
{
	if (input[i] == '[')
	{
		return (handle_brackets(input, i, tokens, env_list));
	}
	return (-1);
}

int	handle_backslash_token(char *input, int i, t_token **tokens)
{
	char	*current_value;
	int		result;

	current_value = NULL;
	result = 0;
	if (input[i] == '\\')
	{
		current_value = ft_strdup("");
		if (!current_value)
			return (-1);
		result = handle_backslash(input, i, &current_value);
		if (result == -1)
		{
			free(current_value);
			return (-1);
		}
		add_token(tokens, create_token(current_value, TYPE_WORD, 0));
		free(current_value);
		return (result);
	}
	return (-1);
}
