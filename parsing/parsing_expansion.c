/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:10:22 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 02:15:46 by akhamass         ###   ########.fr       */
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

char	*handle_var_expansion(t_var_exp *ctx)
{
	char	*var_name;
	char	*var_value;

	var_name = extract_variable_name(ctx->input, ctx->index);
	var_value = get_env_variable(ctx->env_list, var_name);
	free(var_name);
	if (ctx->in_quotes)
	{
		if (var_value)
			return (ft_strdup(var_value));
		else
			return (ft_strdup(""));
	}
	else
	{
		if (var_value)
			add_token(ctx->tokens, create_token(var_value, TYPE_ENV_VAR, 0));
		else
			add_token(ctx->tokens, create_token("", TYPE_ENV_VAR, 0));
		return (NULL);
	}
}
