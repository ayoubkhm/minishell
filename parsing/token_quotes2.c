/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 23:08:53 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 23:09:06 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h" 

int	handle_first_token(t_ctx *ctx, int i, char **token_value)
{
	i = process_first_token(ctx, i, token_value);
	if (i == -1)
	{
		free(*token_value);
		return (-1);
	}
	add_token(ctx->tok, create_token(*token_value, TYPE_WORD, 1));
	return (i);
}

void	handle_standard_token(t_ctx *ctx, char *token_value)
{
	add_token(ctx->tok, create_token(token_value, TYPE_WORD, 0));
}

void	finalize_token_if_first(t_ctx *ctx, int j)
{
	if (j == 0)
		finalize_first_token(ctx);
}

int	handle_tok_proc(t_ctx *ctx, char **split_values, int j, int i)
{
	char	*token_value;

	token_value = get_token_value(ctx, split_values, j);
	if (!token_value)
		return (i);
	if (j == 0)
	{
		i = handle_first_token(ctx, i, &token_value);
		if (i == -1)
			return (-1);
	}
	else
	{
		handle_standard_token(ctx, token_value);
	}
	free(token_value);
	finalize_token_if_first(ctx, j);
	return (i);
}
