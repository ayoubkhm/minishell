/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   indexing_and_validation.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 00:58:33 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 02:43:51 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	validate_start_index(t_ctx *ctx, int start)
{
	if (start < 0 || start >= (int)ft_strlen(ctx->inp))
	{
		free(ctx->dol_seq);
		ctx->dol_seq = NULL;
		return (-1);
	}
	return (0);
}

int	find_end_index(t_ctx *ctx)
{
	int	start;
	int	j;

	start = ctx->i - ft_strlen(ctx->dol_seq);
	j = ctx->i;
	while (ctx->inp[j] && !isspace(ctx->inp[j]) && !is_operator(ctx->inp[j]))
	{
		j++;
	}
	if (j - start <= 0)
	{
		free(ctx->dol_seq);
		ctx->dol_seq = NULL;
		return (-1);
	}
	return (j);
}
