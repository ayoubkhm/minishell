/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:10:28 by akhamass          #+#    #+#             */
/*   Updated: 2024/10/27 12:10:29 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	handle_single_quotes(char *input, int i, t_token **tokens)
{
	int		start;
	char	*value;

	start = i + 1;
	while (input[i] && input[i] != '\'')
	{
		i++;
	}
	if (input[i] != '\'')
	{
		fprintf(stderr, "minishell: syntax error: unclosed single quote\n");
		return (i);
	}
	value = ft_substr(input, start + 1, i - start - 1);
	add_token(tokens, create_token(value, TYPE_QUOTED));
	free(value);
	return (i + 1);
}

char	*append_variable_value(char *value, char *inp, int *i, t_token **tok)
{
	char	*var_value;
	char	*temp;

	var_value = handle_variable_expansion(inp, i, 1, tok);
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

char	*build_double_quoted_string(char *input, int *i, t_token **tokens)
{
	char	*value;

	value = ft_strdup("");
	while (input[*i] && input[*i] != '"')
	{
		if (input[*i] == '$')
		{
			value = append_variable_value(value, input, i, tokens);
		}
		else
		{
			value = append_character(value, input[*i]);
			(*i)++;
		}
	}
	return (value);
}

int	handle_double_quotes(char *input, int i, t_token **tokens)
{
	char	*value;

	i = i + 1;
	value = build_double_quoted_string(input, &i, tokens);
	if (input[i] != '"')
	{
		fprintf(stderr, "minishell: syntax error: unclosed double quote\n");
		free(value);
		return (i);
	}
	i++;
	add_token(tokens, create_token(value, TYPE_QUOTED));
	free(value);
	return (i);
}
