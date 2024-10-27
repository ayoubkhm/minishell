/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:10:22 by akhamass          #+#    #+#             */
/*   Updated: 2024/10/27 12:10:23 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*extract_variable_name(char *input, int *i)
{
	int		start;
	int		var_len;
	char	*var_name;

	start = *i + 1;
	var_len = 0;
	while (input[start + var_len]
		&& (ft_isalnum(input[start + var_len])
			|| input[start + var_len] == '_'))
		var_len++;
	var_name = ft_substr(input, start, var_len);
	*i = start + var_len;
	return (var_name);
}

char	*handle_variable_expansion(char *inp, int *i, int quo, t_token **tokens)
{
	char	*var_name;
	char	*var_value;

	var_name = extract_variable_name(inp, i);
	var_value = getenv(var_name);
	free(var_name);
	if (quo)
	{
		if (var_value)
			return (ft_strdup(var_value));
		else
			return (ft_strdup(""));
	}
	else
	{
		if (var_value)
			add_token(tokens, create_token(var_value, TYPE_ENV_VAR));
		else
			add_token(tokens, create_token("", TYPE_ENV_VAR));
		return (NULL);
	}
}

int	handle_variable_reference(char *input, int i, t_token **tokens)
{
	char	*var_ref;
	int		start;

	start = i;
	i++;
	while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
		i++;
	var_ref = ft_substr(input, start, i - start);
	add_token(tokens, create_token(var_ref, TYPE_ENV_VAR));
	free(var_ref);
	return (i);
}
