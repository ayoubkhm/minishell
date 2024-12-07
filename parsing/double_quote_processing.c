/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_quote_processing.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:38:23 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 02:38:24 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*process_regular_character(t_var_exp *ctx, char *value)
{
	value = append_character(value, ctx->input[*ctx->index]);
	(*ctx->index)++;
	return (value);
}

char	*process_double_quote_characters(t_var_exp *ctx, char *value)
{
	if (ctx->input[*ctx->index] == '$')
		value = process_dollar_sign(ctx, value);
	else
		value = process_regular_character(ctx, value);
	return (value);
}

char	*process_double_quote_content(t_var_exp *ctx)
{
	char	*value;

	value = ft_strdup("");
	if (!value)
		return (NULL);
	while (ctx->input[*ctx->index] && ctx->input[*ctx->index] != '"')
	{
		value = process_double_quote_characters(ctx, value);
		if (!value)
			return (NULL);
	}
	return (value);
}

int	init_var_exp_struc(t_var_exp *ctx, t_var_exp_params *params)
{
	ctx->input = params->input;
	ctx->index = params->index;
	ctx->in_quotes = 1;
	ctx->tokens = params->tokens;
	ctx->env_list = params->env_list;
	*(params->index) += 1;
	return (1);
}
