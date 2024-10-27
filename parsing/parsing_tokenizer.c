/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tokenizer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:10:34 by akhamass          #+#    #+#             */
/*   Updated: 2024/10/27 12:17:37 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	process_token(char *input, int i, t_token **tokens)
{
	if (input[i] == '\'')
	{
		return (handle_single_quotes(input, i, tokens));
	}
	else if (input[i] == '"')
	{
		return (handle_double_quotes(input, i, tokens));
	}
	else if (input[i] == '$')
	{
		return (handle_variable_reference(input, i, tokens));
	}
	else if (is_operator(input[i]))
	{
		return (handle_operator(input, i, tokens));
	}
	else
	{
		return (handle_word(input, i, tokens));
	}
}

t_token	*tokenize_input(char *input)
{
	int		i;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	while (input[i])
	{
		if (isspace(input[i]))
		{
			i++;
			continue ;
		}
		i = process_token(input, i, &tokens);
	}
	return (tokens);
}

int	handle_operator(char *input, int i, t_token **tokens)
{
	char	*operator_str;
	int		operator_type;
	int		shift;

	shift = detect_operator(input, i, &operator_str, &operator_type);
	if (shift > 0)
	{
		add_token(tokens, create_token(operator_str, operator_type));
		return (i + shift);
	}
	return (i);
}

int	handle_word(char *input, int i, t_token **tokens)
{
	int		start;
	char	*word;

	start = i;
	while (input[i] && !isspace(input[i])
		&& !is_operator(input[i])
		&& input[i] != '\''
		&& input[i] != '"')
		i++;
	word = ft_substr(input, start, i - start);
	add_token(tokens, create_token(word, TYPE_WORD));
	free(word);
	return (i);
}
