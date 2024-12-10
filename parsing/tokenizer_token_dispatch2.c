/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_token_dispatch2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 03:02:15 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/10 12:33:58 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	proc_special_tok(char *input, int i, t_token **tokens, t_env *env_list)
{
	int	result;

	result = handle_variable_token(input, i, tokens, env_list);
	if (result != -1)
		return (result);
	result = handle_operator_token(input, i, tokens);
	if (result != -1)
		return (result);
	return (-1);
}

int	proc_complex_tok(char *input, int i, t_token **tokens, t_env *env_list)
{
	int	result;

	result = handle_quotes_token(input, i, tokens, env_list);
	if (result != -1)
		return (result);
	result = handle_brackets_token(input, i, tokens, env_list);
	if (result != -1)
		return (result);
	result = handle_backslash_token(input, i, tokens);
	if (result != -1)
		return (result);
	return (-1);
}

int	handle_operator(char *input, int i, t_token **tokens)
{
	char	*operator_str;
	int		operator_type;
	int		shift;

	shift = detect_operator(input, i, &operator_str, &operator_type);
	if (shift > 0)
	{
		add_token(tokens, create_token(operator_str, operator_type, 0));
		return (i + shift);
	}
	return (i);
}

int	add_variable_token(char *name, char *value, t_token **tokens, int i)
{
	char	*final_variable;
	char	*result;

	final_variable = ft_strjoin(name, "=");
	result = ft_strjoin(final_variable, value);
	if (!final_variable || !result)
	{
		free(final_variable);
		free(result);
		return (-1);
	}
	free(final_variable);
	add_token(tokens, create_token(result, TYPE_WORD, 1));
	free(result);
	i++;
	return (i);
}

int	extract_indices(char *input, int i, t_variable_data *data)
{
	char	quote_char;

	if (input[i + 1] == '"' || input[i + 1] == '\'')
		quote_char = input[i + 1];
	else
	{
		return (-1);
	}
	i += 2;
	data->val_start = i;
	while (input[i] && input[i] != quote_char)
	{
		i++;
	}
	if (!input[i])
	{
		return (-1);
	}
	data->end = i;
	return (0);
}
