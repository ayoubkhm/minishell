/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:02:46 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 18:13:35 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	add_all_tokens(t_ctx *ctx, char **split_values, int i)
{
	int	j;

	j = 0;
	while (split_values[j])
	{
		i = handle_tok_proc(ctx, split_values, j, i);
		if (i == -1)
			return (-1);
		j++;
	}
	return (i);
}

int	gere_valid_var(t_ctx *ctx)
{
	char	**split_values;
	int		i;

	i = ctx->i;
	split_values = xtrct_split_var_val(ctx->inp, &i, ctx->e_l, ctx->dol_seq);
	if (!split_values)
		return (i);
	i = add_all_tokens(ctx, split_values, i);
	if (i == -1)
		return (-1);
	free_split_values(split_values);
	return (i);
}

int	cree_et_add_tok_lit(t_ctx *ctx, int start, int end)
{
	char	*literal_value;

	literal_value = ft_substr(ctx->inp, start, end - start);
	if (!literal_value)
	{
		free(ctx->dol_seq);
		return (-1);
	}
	add_token(ctx->tok, create_token(literal_value, TYPE_WORD, 1));
	free(literal_value);
	free(ctx->dol_seq);
	ctx->dol_seq = NULL;
	return (end);
}

int	handle_invalid_variable(t_ctx *ctx)
{
	int	start;
	int	end;

	start = ctx->i - ft_strlen(ctx->dol_seq);
	if (validate_start_index(ctx, start) == -1)
		return (-1);
	end = find_end_index(ctx);
	if (end == -1)
		return (-1);
	return (cree_et_add_tok_lit(ctx, start, end));
}
