/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:42:48 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 02:42:49 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	handle_quotes_in_var(char *inp, int i, char **token_value)
{
	char	temp[2];
	char	*new_token_value;

	if (inp[i] == '"')
	{
		i++;
		while (inp[i] && inp[i] != '"')
		{
			temp[0] = inp[i];
			temp[1] = '\0';
			new_token_value = ft_strjoin(*token_value, temp);
			free(*token_value);
			*token_value = new_token_value;
			i++;
		}
		if (inp[i] == '"')
			i++;
		else
		{
			free(*token_value);
			return (-1);
		}
	}
	return (i);
}

char	*get_token_value(t_ctx *ctx, char **split_values, int j)
{
	if (j == 0)
		return (bild_cmbined_val(ctx->dol_cnt, ctx->dol_seq, split_values[j]));
	return (ft_strdup(split_values[j]));
}

int	process_first_token(t_ctx *ctx, int i, char **token_value)
{
	i = handle_quotes_in_var(ctx->inp, i, token_value);
	if (i == -1)
	{
		free(ctx->dol_seq);
		ctx->dol_seq = NULL;
	}
	return (i);
}

void	finalize_first_token(t_ctx *ctx)
{
	free(ctx->dol_seq);
	ctx->dol_seq = NULL;
}

int	handle_tok_proc(t_ctx *ctx, char **split_values, int j, int i)
{
	char	*token_value;

	token_value = get_token_value(ctx, split_values, j);
	if (!token_value)
		return (i);
	if (j == 0)
	{
		i = process_first_token(ctx, i, &token_value);
		if (i == -1)
		{
			free(token_value);
			return (-1);
		}
	}
	add_token(ctx->tok, create_token(token_value, TYPE_WORD, 1));
	free(token_value);
	if (j == 0)
		finalize_first_token(ctx);
	return (i);
}
