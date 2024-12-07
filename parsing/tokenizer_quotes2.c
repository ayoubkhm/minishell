/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_quotes2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 03:04:54 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 03:04:55 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*process_all_quotes(t_quote_context *ctx)
{
	char	quote_type;

	while (ctx->input[*(ctx->i)] == '\'' || ctx->input[*(ctx->i)] == '"')
	{
		quote_type = ctx->input[*(ctx->i)];
		ctx->final_content = process_quoted_section(ctx, quote_type);
		if (!ctx->final_content)
		{
			return (NULL);
		}
	}
	return (ctx->final_content);
}

int	gere_var_quotes(char *input, int i, t_token **tokens, t_env *env_list)
{
	t_quote_context	ctx;

	ctx.input = input;
	ctx.i = &i;
	ctx.env_list = env_list;
	ctx.final_content = ft_strdup("");
	if (!ctx.final_content)
	{
		return (-1);
	}
	ctx.final_content = process_all_quotes(&ctx);
	if (!ctx.final_content)
	{
		return (-1);
	}
	ctx.final_content = gere_adjcnt_txt(ctx.input, ctx.i, ctx.final_content);
	if (!ctx.final_content)
	{
		return (-1);
	}
	add_token(tokens, create_token(ctx.final_content, TYPE_WORD, 0));
	free(ctx.final_content);
	return (i);
}

int	check_and_gere_quotes(char *input, int i, t_token **tok, t_env *env_list)
{
	if (i > 0 && (input[i] == '\'' || input[i] == '"'))
		return (gere_var_quotes(input, i, tok, env_list));
	return (-1);
}

char	*remove_quotes(const char *input)
{
	char	*cleaned;
	int		j;
	int		i;

	cleaned = malloc(strlen(input) + 1);
	j = 0;
	i = 0;
	if (!cleaned)
	{
		return (NULL);
	}
	while (input[i])
	{
		if (input[i] != '\'' && input[i] != '"')
		{
			cleaned[j] = input[i];
			j++;
		}
		i++;
	}
	cleaned[j] = '\0';
	return (cleaned);
}
