/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_variables3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 05:28:51 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/10 02:19:26 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	gere_num_var(t_ctx *ctx)
{
	char	*token_value;

	token_value = initialize_var_value(ctx->inp, ctx->i, ctx->e_l);
	if (!token_value)
	{
		free(ctx->dol_seq);
		return (-1);
	}
	ctx->i++;
	token_value = accumulate_invalid_chars(ctx->inp, &(ctx->i), token_value);
	if (!token_value)
	{
		free(ctx->dol_seq);
		return (-1);
	}
	return (finalize_and_add_tok(ctx->tok, token_value, ctx->dol_seq, ctx->i));
}

int	accumulate_dol_seq(char *inp, int i, char **dol_seq, int *dol_cnt)
{
	int	start;

	start = i;
	*dol_seq = NULL;
	*dol_cnt = 0;
	while (inp[i] == '$')
	{
		(*dol_cnt)++;
		i++;
	}
	*dol_seq = ft_substr(inp, start, i - start);
	if (!*dol_seq)
	{
		return (-1);
	}
	return (i);
}
