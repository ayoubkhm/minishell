/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:10:22 by akhamass          #+#    #+#             */
/*   Updated: 2024/11/13 05:41:41 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char *append_string(char *original, char *addition)
{
    char *temp = ft_strjoin(original, addition);
    free(original);
    return temp;
}


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

char	*handle_variable_expansion(char *input, int *i, int in_quotes, t_token **tokens, t_env *env_list)
{
	char	*var_name;
	char	*var_value;

	var_name = extract_variable_name(input, i);
	var_value = get_env_variable(env_list, var_name);
	free(var_name);

	if (in_quotes)
	{
		if (var_value)
			return ft_strdup(var_value);
		else
			return ft_strdup("");
	}
	else
	{
		if (var_value)
			add_token(tokens, create_token(var_value, TYPE_ENV_VAR, 0));
		else
			add_token(tokens, create_token("", TYPE_ENV_VAR, 0));
		return NULL;
	}
}
