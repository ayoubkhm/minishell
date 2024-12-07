/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_variables4.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 23:11:21 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 23:11:23 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	combine_prefix_with_var(t_pars_cxt *ctx, t_token *first_var_token)
{
	char	*combined;

	combined = ft_strjoin(ctx->pfx, first_var_token->value);
	free(ctx->pfx);
	ctx->pfx = NULL;
	free(first_var_token->value);
	first_var_token->value = combined;
	first_var_token->expand = 0;
}

t_token	*find_first_expandable_token(t_token *token)
{
	while (token)
	{
		if (token->expand == 1)
			return (token);
		token = token->next;
	}
	return (NULL);
}

void	handle_prefix_logic(t_pars_cxt *ctx)
{
	t_token	*first_var_token;

	first_var_token = find_first_expandable_token(*(ctx->tok));
	if (first_var_token)
		combine_prefix_with_var(ctx, first_var_token);
	else
	{
		free(ctx->pfx);
		ctx->pfx = NULL;
	}
}

void	reset_expand_flags(t_token *token)
{
	while (token)
	{
		if (token->expand != 0)
			token->expand = 0;
		token = token->next;
	}
}

int	gere_var2(t_pars_cxt *ctx)
{
	ctx->i = gere_var_ref(ctx->inp, ctx->i, ctx->tok, ctx->e_l);
	if (*(ctx->tok) == NULL || (*(ctx->tok))->value == NULL)
	{
		free(ctx->pfx);
		ctx->pfx = NULL;
		return (ctx->i);
	}
	if (ctx->pfx && *(ctx->pfx))
		handle_prefix_logic(ctx);
	reset_expand_flags(*(ctx->tok));
	return (ctx->i);
}
