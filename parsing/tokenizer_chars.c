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

char	*extract_prefix(char *input, int start, int i)
{
	char	*prefix;
	char	*cleaned;

	if (start < i)
	{
		prefix = ft_substr(input, start, i - start);
		if (!prefix)
			return (NULL);
		cleaned = remove_quotes(prefix);
		free(prefix);
		return (cleaned);
	}
	return (NULL);
}

void	process_prefix_as_word(char *prefix, t_token **tokens)
{
	if (prefix && *prefix)
	{
		add_token(tokens, create_token(prefix, TYPE_WORD, 0));
	}
}
