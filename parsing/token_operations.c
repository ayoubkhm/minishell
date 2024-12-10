/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:02:46 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/10 00:47:08 by akhamass         ###   ########.fr       */
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

	printf("[DEBUG] gere_valid_var: start, ctx->i=%d, ctx->inp + ctx->i='%s'\n", ctx->i, ctx->inp + ctx->i);

	i = ctx->i;
	split_values = xtrct_split_var_val(ctx->inp, &i, ctx->e_l, ctx->dol_seq);
	if (!split_values)
	{
		printf("[DEBUG] gere_valid_var: xtrct_split_var_val a échoué, retour i=%d\n", i);
		return (i);
	}
	printf("[DEBUG] gere_valid_var: split_values extrait avec succès.\n");
	// On peut debuguer le contenu de split_values
	{
		int j = 0;
		while (split_values[j])
		{
			printf("[DEBUG] split_values[%d] = '%s'\n", j, split_values[j]);
			j++;
		}
	}

	i = add_all_tokens(ctx, split_values, i);
	printf("[DEBUG] gere_valid_var: add_all_tokens retourne i=%d\n", i);
	if (i == -1)
	{
		printf("[DEBUG] gere_valid_var: add_all_tokens a échoué.\n");
		return (-1);
	}

	free_split_values(split_values);
	printf("[DEBUG] gere_valid_var: fin normale, retour i=%d\n", i);
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
